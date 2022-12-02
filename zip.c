/*
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#define __STDC_WANT_LIB_EXT1__ 1

#include <errno.h>
#include <sys/stat.h>
#include <time.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER) ||              \
    defined(__MINGW32__)
/* Win32, DOS, MSVC, MSVS */
#include <direct.h>

#define STRCLONE(STR) ((STR) ? _strdup(STR) : NULL)
#define HAS_DEVICE(P)                                                          \
  ((((P)[0] >= 'A' && (P)[0] <= 'Z') || ((P)[0] >= 'a' && (P)[0] <= 'z')) &&   \
   (P)[1] == ':')
#define FILESYSTEM_PREFIX_LEN(P) (HAS_DEVICE(P) ? 2 : 0)

#else

#include <unistd.h> // needed for symlink()
#define STRCLONE(STR) ((STR) ? strdup(STR) : NULL)

#endif

#ifdef __MINGW32__
#include <sys/types.h>
#include <unistd.h>
#endif

#include "miniz.h"
#include "zip.h"

#ifdef _MSC_VER
#include <io.h>

#define ftruncate(fd, sz) (-(_chsize_s((fd), (sz)) != 0))
#define fileno _fileno
#endif

#if defined(__TINYC__) && (defined(_WIN32) || defined(_WIN64))
#include <io.h>

#define ftruncate(fd, sz) (-(_chsize_s((fd), (sz)) != 0))
#define fileno _fileno
#endif

#ifndef HAS_DEVICE
#define HAS_DEVICE(P) 0
#endif

#ifndef FILESYSTEM_PREFIX_LEN
#define FILESYSTEM_PREFIX_LEN(P) 0
#endif

#ifndef ISSLASH
#define ISSLASH(C) ((C) == '/' || (C) == '\\')
#endif

#define CLEANUP(ptr)                                                           \
  do {                                                                         \
    if (ptr) {                                                                 \
      free((void *)ptr);                                                       \
      ptr = NULL;                                                              \
    }                                                                          \
  } while (0)

#define UNX_IFDIR 0040000  /* Unix directory */
#define UNX_IFREG 0100000  /* Unix regular file */
#define UNX_IFSOCK 0140000 /* Unix socket (BSD, not SysV or Amiga) */
#define UNX_IFLNK 0120000  /* Unix symbolic link (not SysV, Amiga) */
#define UNX_IFBLK 0060000  /* Unix block special       (not Amiga) */
#define UNX_IFCHR 0020000  /* Unix character special   (not Amiga) */
#define UNX_IFIFO 0010000  /* Unix fifo    (BCC, not MSC or Amiga) */

struct zip_entry_t {
  ssize_t index;
  char *name;
  mz_uint64 uncomp_size;
  mz_uint64 comp_size;
  mz_uint32 uncomp_crc32;
  mz_uint64 offset;
  mz_uint8 header[MZ_ZIP_LOCAL_DIR_HEADER_SIZE];
  mz_uint64 header_offset;
  mz_uint16 method;
  mz_zip_writer_add_state state;
  tdefl_compressor comp;
  mz_uint32 external_attr;
  time_t m_time;
};

struct zip_t {
  mz_zip_archive archive;
  mz_uint level;
  struct zip_entry_t entry;
};

enum zip_modify_t {
  MZ_KEEP = 0,
  MZ_DELETE = 1,
  MZ_MOVE = 2,
};

struct zip_entry_mark_t {
  ssize_t file_index;
  enum zip_modify_t type;
  mz_uint64 m_local_header_ofs;
  size_t lf_length;
};

static const char *const zip_errlist[30] = {
    NULL,
    "not initialized\0",
    "invalid entry name\0",
    "entry not found\0",
    "invalid zip mode\0",
    "invalid compression level\0",
    "no zip 64 support\0",
    "memset error\0",
    "cannot write data to entry\0",
    "cannot initialize tdefl compressor\0",
    "invalid index\0",
    "header not found\0",
    "cannot flush tdefl buffer\0",
    "cannot write entry header\0",
    "cannot create entry header\0",
    "cannot write to central dir\0",
    "cannot open file\0",
    "invalid entry type\0",
    "extracting data using no memory allocation\0",
    "file not found\0",
    "no permission\0",
    "out of memory\0",
    "invalid zip archive name\0",
    "make dir error\0",
    "symlink error\0",
    "close archive error\0",
    "capacity size too small\0",
    "fseek error\0",
    "fread error\0",
    "fwrite error\0",
};

const char *zip_strerror(int errnum) {
  errnum = -errnum;
  if (errnum <= 0 || errnum >= 30) {
    return NULL;
  }

  return zip_errlist[errnum];
}

static const char *zip_basename(const char *name) {
  char const *p;
  char const *base = name += FILESYSTEM_PREFIX_LEN(name);
  int all_slashes = 1;

  for (p = name; *p; p++) {
    if (ISSLASH(*p))
      base = p + 1;
    else
      all_slashes = 0;
  }

  /* If NAME is all slashes, arrange to return `/'. */
  if (*base == '\0' && ISSLASH(*name) && all_slashes)
    --base;

  return base;
}

static int zip_mkpath(char *path) {
  char *p;
  char npath[MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE + 1];
  int len = 0;
  int has_device = HAS_DEVICE(path);

  memset(npath, 0, MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE + 1);
  if (has_device) {
    // only on windows
    npath[0] = path[0];
    npath[1] = path[1];
    len = 2;
  }
  for (p = path + len; *p && len < MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE; p++) {
    if (ISSLASH(*p) && ((!has_device && len > 0) || (has_device && len > 2))) {
#if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER) ||              \
    defined(__MINGW32__)
#else
      if ('\\' == *p) {
        *p = '/';
      }
#endif

      if (MZ_MKDIR(npath) == -1) {
        if (errno != EEXIST) {
          return ZIP_EMKDIR;
        }
      }
    }
    npath[len++] = *p;
  }

  return 0;
}

static char *zip_strrpl(const char *str, size_t n, char oldchar, char newchar) {
  char c;
  size_t i;
  char *rpl = (char *)calloc((1 + n), sizeof(char));
  char *begin = rpl;
  if (!rpl) {
    return NULL;
  }

  for (i = 0; (i < n) && (c = *str++); ++i) {
    if (c == oldchar) {
      c = newchar;
    }
    *rpl++ = c;
  }

  return begin;
}

static char *zip_name_normalize(char *name, char *const nname, size_t len) {
  size_t offn = 0;
  size_t offnn = 0, ncpy = 0;

  if (name == NULL || nname == NULL || len <= 0) {
    return NULL;
  }
  // skip trailing '/'
  while (ISSLASH(*name))
    name++;

  for (; offn < len; offn++) {
    if (ISSLASH(name[offn])) {
      if (ncpy > 0 && strcmp(&nname[offnn], ".\0") &&
          strcmp(&nname[offnn], "..\0")) {
        offnn += ncpy;
        nname[offnn++] = name[offn]; // append '/'
      }
      ncpy = 0;
    } else {
      nname[offnn + ncpy] = name[offn];
      ncpy++;
    }
  }

  // at the end, extra check what we've already copied
  if (ncpy == 0 || !strcmp(&nname[offnn], ".\0") ||
      !strcmp(&nname[offnn], "..\0")) {
    nname[offnn] = 0;
  }
  return nname;
}

static mz_bool zip_name_match(const char *name1, const char *name2) {
  char *nname2 = NULL;

#ifdef ZIP_RAW_ENTRYNAME
  nname2 = STRCLONE(name2);
#else
  nname2 = zip_strrpl(name2, strlen(name2), '\\', '/');
#endif

  if (!nname2) {
    return MZ_FALSE;
  }

  mz_bool res = (strcmp(name1, nname2) == 0) ? MZ_TRUE : MZ_FALSE;
  CLEANUP(nname2);
  return res;
}

static int zip_archive_truncate(mz_zip_archive *pzip) {
  mz_zip_internal_state *pState = pzip->m_pState;
  mz_uint64 file_size = pzip->m_archive_size;
  if ((pzip->m_pWrite == mz_zip_heap_write_func) && (pState->m_pMem)) {
    return 0;
  }
  if (pzip->m_zip_mode == MZ_ZIP_MODE_WRITING_HAS_BEEN_FINALIZED) {
    if (pState->m_pFile) {
      int fd = fileno(pState->m_pFile);
      return ftruncate(fd, file_size);
    }
  }
  return 0;
}

static int zip_archive_extract(mz_zip_archive *zip_archive, const char *dir,
                               int (*on_extract)(const char *filename,
                                                 void *arg),
                               void *arg) {
  int err = 0;
  mz_uint i, n;
  char path[MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE + 1];
  char symlink_to[MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE + 1];
  mz_zip_archive_file_stat info;
  size_t dirlen = 0, filename_size = MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE;
  mz_uint32 xattr = 0;

  memset(path, 0, sizeof(path));
  memset(symlink_to, 0, sizeof(symlink_to));

  dirlen = strlen(dir);
  if (dirlen + 1 > MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE) {
    return ZIP_EINVENTNAME;
  }

  memset((void *)&info, 0, sizeof(mz_zip_archive_file_stat));

#if defined(_MSC_VER)
  strcpy_s(path, MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE, dir);
#else
  strcpy(path, dir);
#endif

  if (!ISSLASH(path[dirlen - 1])) {
#if defined(_WIN32) || defined(__WIN32__)
    path[dirlen] = '\\';
#else
    path[dirlen] = '/';
#endif
    ++dirlen;
  }

  if (filename_size > MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE - dirlen) {
    filename_size = MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE - dirlen;
  }
  // Get and print information about each file in the archive.
  n = mz_zip_reader_get_num_files(zip_archive);
  for (i = 0; i < n; ++i) {
    if (!mz_zip_reader_file_stat(zip_archive, i, &info)) {
      // Cannot get information about zip archive;
      err = ZIP_ENOENT;
      goto out;
    }

    if (!zip_name_normalize(info.m_filename, info.m_filename,
                            strlen(info.m_filename))) {
      // Cannot normalize file name;
      err = ZIP_EINVENTNAME;
      goto out;
    }

#if defined(_MSC_VER)
    strncpy_s(&path[dirlen], filename_size, info.m_filename,
              filename_size);
#else
    strncpy(&path[dirlen], info.m_filename, filename_size);
#endif
    err = zip_mkpath(path);
    if (err < 0) {
      // Cannot make a path
      goto out;
    }

    if ((((info.m_version_made_by >> 8) == 3) ||
         ((info.m_version_made_by >> 8) ==
          19)) // if zip is produced on Unix or macOS (3 and 19 from
               // section 4.4.2.2 of zip standard)
        && info.m_external_attr &
               (0x20 << 24)) { // and has sym link attribute (0x80 is file, 0x40
                               // is directory)
#if defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER) ||              \
    defined(__MINGW32__)
#else
      if (info.m_uncomp_size > MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE ||
          !mz_zip_reader_extract_to_mem_no_alloc(zip_archive, i, symlink_to,
                                                 MZ_ZIP_MAX_ARCHIVE_FILENAME_SIZE, 0, NULL, 0)) {
        err = ZIP_EMEMNOALLOC;
        goto out;
      }
      symlink_to[info.m_uncomp_size] = '\0';
      if (symlink(symlink_to, path) != 0) {
        err = ZIP_ESYMLINK;
        goto out;
      }
#endif
    } else {
      if (!mz_zip_reader_is_file_a_directory(zip_archive, i)) {
        if (!mz_zip_reader_extract_to_file(zip_archive, i, path, 0)) {
          // Cannot extract zip archive to file
          err = ZIP_ENOFILE;
          goto out;
        }
      }

#if defined(_MSC_VER) || defined(PS4)
      (void)xattr; // unused
#else
      xattr = (info.m_external_attr >> 16) & 0xFFFF;
      if (xattr > 0 && xattr <= MZ_UINT16_MAX) {
        if (CHMOD(path, (mode_t)xattr) < 0) {
          err = ZIP_ENOPERM;
          goto out;
        }
      }
#endif
    }

    if (on_extract) {
      if (on_extract(path, arg) < 0) {
        goto out;
      }
    }
  }

out:
  // Close the archive, freeing any resources it was using
  if (!mz_zip_reader_end(zip_archive)) {
    // Cannot end zip reader
    err = ZIP_ECLSZIP;
  }
  return err;
}

static inline void zip_archive_finalize(mz_zip_archive *pzip) {
  mz_zip_writer_finalize_archive(pzip);
  zip_archive_truncate(pzip);
}

static ssize_t zip_entry_mark(struct zip_t *zip,
                              struct zip_entry_mark_t *entry_mark,
                              const ssize_t n, char *const entries[],
                              const size_t len) {
  ssize_t i = 0;
  ssize_t err = 0;
  if (!zip || !entry_mark || !entries) {
    return ZIP_ENOINIT;
  }

  mz_zip_archive_file_stat file_stat;
  mz_uint64 d_pos = ~0UL;
  for (i = 0; i < n; ++i) {
    if ((err = zip_entry_openbyindex(zip, i))) {
      return (ssize_t)err;
    }

    mz_bool name_matches = MZ_FALSE;
    {
      size_t j;
      for (j = 0; j < len; ++j) {
        if (zip_name_match(zip->entry.name, entries[j])) {
          name_matches = MZ_TRUE;
          break;
        }
      }
    }
    if (name_matches) {
      entry_mark[i].type = MZ_DELETE;
    } else {
      entry_mark[i].type = MZ_KEEP;
    }

    if (!mz_zip_reader_file_stat(&zip->archive, i, &file_stat)) {
      return ZIP_ENOENT;
    }

    zip_entry_close(zip);

    entry_mark[i].m_local_header_ofs = file_stat.m_local_header_ofs;
    entry_mark[i].file_index = (ssize_t)-1;
    entry_mark[i].lf_length = 0;
    if ((entry_mark[i].type) == MZ_DELETE &&
        (d_pos > entry_mark[i].m_local_header_ofs)) {
      d_pos = entry_mark[i].m_local_header_ofs;
    }
  }

  for (i = 0; i < n; ++i) {
    if ((entry_mark[i].m_local_header_ofs > d_pos) &&
        (entry_mark[i].type != MZ_DELETE)) {
      entry_mark[i].type = MZ_MOVE;
    }
  }
  return err;
}

static ssize_t zip_index_next(mz_uint64 *local_header_ofs_array,
                              ssize_t cur_index) {
  ssize_t new_index = 0, i;
  for (i = cur_index - 1; i >= 0; --i) {
    if (local_header_ofs_array[cur_index] > local_header_ofs_array[i]) {
      new_index = i + 1;
      return new_index;
    }
  }
  return new_index;
}

static ssize_t zip_sort(mz_uint64 *local_header_ofs_array, ssize_t cur_index) {
  ssize_t nxt_index = zip_index_next(local_header_ofs_array, cur_index);

  if (nxt_index != cur_index) {
    mz_uint64 temp = local_header_ofs_array[cur_index];
    ssize_t i;
    for (i = cur_index; i > nxt_index; i--) {
      local_header_ofs_array[i] = local_header_ofs_array[i - 1];
    }
    local_header_ofs_array[nxt_index] = temp;
  }
  return nxt_index;
}

static int zip_index_update(struct zip_entry_mark_t *entry_mark,
                            ssize_t last_index, ssize_t nxt_index) {
  ssize_t j;
  for (j = 0; j < last_index; j++) {
    if (entry_mark[j].file_index >= nxt_index) {
      entry_mark[j].file_index += 1;
    }
  }
  entry_mark[nxt_index].file_index = last_index;
  return 0;
}

static int zip_entry_finalize(struct zip_t *zip,
                              struct zip_entry_mark_t *entry_mark,
                              const ssize_t n) {

  ssize_t i = 0;
  mz_uint64 *local_header_ofs_array = (mz_uint64 *)calloc(n, sizeof(mz_uint64));
  if (!local_header_ofs_array) {
    return ZIP_EOOMEM;
  }

  for (i = 0; i < n; ++i) {
    local_header_ofs_array[i] = entry_mark[i].m_local_header_ofs;
    ssize_t index = zip_sort(local_header_ofs_array, i);

    if (index != i) {
      zip_index_update(entry_mark, i, index);
    }
    entry_mark[i].file_index = index;
  }

  size_t *length = (size_t *)calloc(n, sizeof(size_t));
  if (!length) {
    CLEANUP(local_header_ofs_array);
    return ZIP_EOOMEM;
  }
  for (i = 0; i < n - 1; i++) {
    length[i] =
        (size_t)(local_header_ofs_array[i + 1] - local_header_ofs_array[i]);
  }
  length[n - 1] =
      (size_t)(zip->archive.m_archive_size - local_header_ofs_array[n - 1]);

  for (i = 0; i < n; i++) {
    entry_mark[i].lf_length = length[entry_mark[i].file_index];
  }

  CLEANUP(length);
  CLEANUP(local_header_ofs_array);
  return 0;
}

static ssize_t zip_entry_set(struct zip_t *zip,
                             struct zip_entry_mark_t *entry_mark, ssize_t n,
                             char *const entries[], const size_t len) {
  ssize_t err = 0;

  if ((err = zip_entry_mark(zip, entry_mark, n, entries, len)) < 0) {
    return err;
  }
  if ((err = zip_entry_finalize(zip, entry_mark, n)) < 0) {
    return err;
  }
  return 0;
}

static ssize_t zip_file_move(MZ_FILE *m_pFile, const mz_uint64 to,
                             const mz_uint64 from, const size_t length,
                             mz_uint8 *move_buf, const size_t capacity_size) {
  if (length > capacity_size) {
    return ZIP_ECAPSIZE;
  }
  if (MZ_FSEEK64(m_pFile, from, SEEK_SET)) {
    return ZIP_EFSEEK;
  }
  if (fread(move_buf, 1, length, m_pFile) != length) {
    return ZIP_EFREAD;
  }
  if (MZ_FSEEK64(m_pFile, to, SEEK_SET)) {
    return ZIP_EFSEEK;
  }
  if (fwrite(move_buf, 1, length, m_pFile) != length) {
    return ZIP_EFWRITE;
  }
  return (ssize_t)length;
}

static ssize_t zip_files_move(MZ_FILE *m_pFile, mz_uint64 writen_num,
                              mz_uint64 read_num, size_t length) {
  ssize_t n = 0;
  const size_t page_size = 1 << 12; // 4K
  mz_uint8 *move_buf = (mz_uint8 *)calloc(1, page_size);
  if (!move_buf) {
    return ZIP_EOOMEM;
  }

  ssize_t moved_length = 0;
  ssize_t move_count = 0;
  while ((mz_int64)length > 0) {
    move_count = (length >= page_size) ? page_size : length;
    n = zip_file_move(m_pFile, writen_num, read_num, move_count, move_buf,
                      page_size);
    if (n < 0) {
      moved_length = n;
      goto cleanup;
    }

    if (n != move_count) {
      goto cleanup;
    }

    writen_num += move_count;
    read_num += move_count;
    length -= move_count;
    moved_length += move_count;
  }

cleanup:
  CLEANUP(move_buf);
  return moved_length;
}

static int zip_central_dir_move(mz_zip_internal_state *pState, int begin,
                                int end, int entry_num) {
  if (begin == entry_num) {
    return 0;
  }

  size_t l_size = 0;
  size_t r_size = 0;
  mz_uint32 d_size = 0;
  mz_uint8 *next = NULL;
  mz_uint8 *deleted = &MZ_ZIP_ARRAY_ELEMENT(
      &pState->m_central_dir, mz_uint8,
      MZ_ZIP_ARRAY_ELEMENT(&pState->m_central_dir_offsets, mz_uint32, begin));
  l_size = (size_t)(deleted - (mz_uint8 *)(pState->m_central_dir.m_p));
  if (end == entry_num) {
    r_size = 0;
  } else {
    next = &MZ_ZIP_ARRAY_ELEMENT(
        &pState->m_central_dir, mz_uint8,
        MZ_ZIP_ARRAY_ELEMENT(&pState->m_central_dir_offsets, mz_uint32, end));
    r_size = pState->m_central_dir.m_size -
             (mz_uint32)(next - (mz_uint8 *)(pState->m_central_dir.m_p));
    d_size = (mz_uint32)(next - deleted);
  }

  if (next && l_size == 0) {
    memmove(pState->m_central_dir.m_p, next, r_size);
    pState->m_central_dir.m_p = MZ_REALLOC(pState->m_central_dir.m_p, r_size);
    {
      int i;
      for (i = end; i < entry_num; i++) {
        MZ_ZIP_ARRAY_ELEMENT(&pState->m_central_dir_offsets, mz_uint32, i) -=
            d_size;
      }
    }
  }

  if (next && l_size * r_size != 0) {
    memmove(deleted, next, r_size);
    {
      int i;
      for (i = end; i < entry_num; i++) {
        MZ_ZIP_ARRAY_ELEMENT(&pState->m_central_dir_offsets, mz_uint32, i) -=
            d_size;
      }
    }
  }

  pState->m_central_dir.m_size = l_size + r_size;
  return 0;
}

static int zip_central_dir_delete(mz_zip_internal_state *pState,
                                  int *deleted_entry_index_array,
                                  int entry_num) {
  int i = 0;
  int begin = 0;
  int end = 0;
  int d_num = 0;
  while (i < entry_num) {
    while ((i < entry_num) && (!deleted_entry_index_array[i])) {
      i++;
    }
    begin = i;

    while ((i < entry_num) && (deleted_entry_index_array[i])) {
      i++;
    }
    end = i;
    zip_central_dir_move(pState, begin, end, entry_num);
  }

  i = 0;
  while (i < entry_num) {
    while ((i < entry_num) && (!deleted_entry_index_array[i])) {
      i++;
    }
    begin = i;
    if (begin == entry_num) {
      break;
    }
    while ((i < entry_num) && (deleted_entry_index_array[i])) {
      i++;
    }
    end = i;
    int k = 0, j;
    for (j = end; j < entry_num; j++) {
      MZ_ZIP_ARRAY_ELEMENT(&pState->m_central_dir_offsets, mz_uint32,
                           begin + k) =
          (mz_uint32)MZ_ZIP_ARRAY_ELEMENT(&pState->m_central_dir_offsets,
                                          mz_uint32, j);
      k++;
    }
    d_num += end - begin;
  }

  pState->m_central_dir_offsets.m_size =
      sizeof(mz_uint32) * (entry_num - d_num);
  return 0;
}

static ssize_t zip_entries_delete_mark(struct zip_t *zip,
                                       struct zip_entry_mark_t *entry_mark,
                                       int entry_num) {
  mz_uint64 writen_num = 0;
  mz_uint64 read_num = 0;
  size_t deleted_length = 0;
  size_t move_length = 0;
  int i = 0;
  size_t deleted_entry_num = 0;
  ssize_t n = 0;

  mz_bool *deleted_entry_flag_array =
      (mz_bool *)calloc(entry_num, sizeof(mz_bool));
  if (deleted_entry_flag_array == NULL) {
    return ZIP_EOOMEM;
  }

  mz_zip_internal_state *pState = zip->archive.m_pState;
  zip->archive.m_zip_mode = MZ_ZIP_MODE_WRITING;

  if ((!pState->m_pFile) || MZ_FSEEK64(pState->m_pFile, 0, SEEK_SET)) {
    CLEANUP(deleted_entry_flag_array);
    return ZIP_ENOENT;
  }

  while (i < entry_num) {
    while ((i < entry_num) && (entry_mark[i].type == MZ_KEEP)) {
      writen_num += entry_mark[i].lf_length;
      read_num = writen_num;
      i++;
    }

    while ((i < entry_num) && (entry_mark[i].type == MZ_DELETE)) {
      deleted_entry_flag_array[i] = MZ_TRUE;
      read_num += entry_mark[i].lf_length;
      deleted_length += entry_mark[i].lf_length;
      i++;
      deleted_entry_num++;
    }

    while ((i < entry_num) && (entry_mark[i].type == MZ_MOVE)) {
      move_length += entry_mark[i].lf_length;
      mz_uint8 *p = &MZ_ZIP_ARRAY_ELEMENT(
          &pState->m_central_dir, mz_uint8,
          MZ_ZIP_ARRAY_ELEMENT(&pState->m_central_dir_offsets, mz_uint32, i));
      if (!p) {
        CLEANUP(deleted_entry_flag_array);
        return ZIP_ENOENT;
      }
      mz_uint32 offset = MZ_READ_LE32(p + MZ_ZIP_CDH_LOCAL_HEADER_OFS);
      offset -= (mz_uint32)deleted_length;
      MZ_WRITE_LE32(p + MZ_ZIP_CDH_LOCAL_HEADER_OFS, offset);
      i++;
    }

    n = zip_files_move(pState->m_pFile, writen_num, read_num, move_length);
    if (n != (ssize_t)move_length) {
      CLEANUP(deleted_entry_flag_array);
      return n;
    }
    writen_num += move_length;
    read_num += move_length;
  }

  zip->archive.m_archive_size -= (mz_uint64)deleted_length;
  zip->archive.m_total_files =
      (mz_uint32)entry_num - (mz_uint32)deleted_entry_num;

  zip_central_dir_delete(pState, deleted_entry_flag_array, entry_num);
  CLEANUP(deleted_entry_flag_array);

  return (ssize_t)deleted_entry_num;
}

struct zip_t *zip_open(const char *zipname, int level, char mode) {
  struct zip_t *zip = NULL;

  if (!zipname || strlen(zipname) < 1) {
    // zip_t archive name is empty or NULL
    goto cleanup;
  }

  if (level < 0)
    level = MZ_DEFAULT_LEVEL;
  if ((level & 0xF) > MZ_UBER_COMPRESSION) {
    // Wrong compression level
    goto cleanup;
  }

  zip = (struct zip_t *)calloc((size_t)1, sizeof(struct zip_t));
  if (!zip)
    goto cleanup;

  zip->level = (mz_uint)level;
  switch (mode) {
  case 'w':
    // Create a new archive.
    if (!mz_zip_writer_init_file_v2(&(zip->archive), zipname, 0,
                                    MZ_ZIP_FLAG_WRITE_ZIP64)) {
      // Cannot initialize zip_archive writer
      goto cleanup;
    }
    break;

  case 'r':
    if (!mz_zip_reader_init_file_v2(
            &(zip->archive), zipname,
            zip->level | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY, 0, 0)) {
      // An archive file does not exist or cannot initialize
      // zip_archive reader
      goto cleanup;
    }
    break;

  case 'a':
  case 'd':
    if (!mz_zip_reader_init_file_v2_rpb(
            &(zip->archive), zipname,
            zip->level | MZ_ZIP_FLAG_DO_NOT_SORT_CENTRAL_DIRECTORY, 0, 0)) {
      // An archive file does not exist or cannot initialize
      // zip_archive reader
      goto cleanup;
    }
    if ((mode == 'a' || mode == 'd')) {
      if (!mz_zip_writer_init_from_reader_v2_noreopen(&(zip->archive), zipname,
                                                      0)) {
        mz_zip_reader_end(&(zip->archive));
        goto cleanup;
      }
    }
    break;

  default:
    goto cleanup;
  }

  return zip;

cleanup:
  CLEANUP(zip);
  return NULL;
}

void zip_close(struct zip_t *zip) {
  if (zip) {
    // Always finalize, even if adding failed for some reason, so we have a
    // valid central directory.
    mz_zip_writer_finalize_archive(&(zip->archive));
    zip_archive_truncate(&(zip->archive));
    mz_zip_writer_end(&(zip->archive));
    mz_zip_reader_end(&(zip->archive));

    CLEANUP(zip);
  }
}

int zip_is64(struct zip_t *zip) {
  if (!zip || !zip->archive.m_pState) {
    // zip_t handler or zip state is not initialized
    return ZIP_ENOINIT;
  }

  return (int)zip->archive.m_pState->m_zip64;
}
