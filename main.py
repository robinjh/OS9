import tkinter
import tkinter as tk
import tkinter.font
from tkinterdnd2 import *
import shutil

from tkinter import TOP, Entry, Label, StringVar

import ctypes                           # 파이썬 extension을 사용하기 위한 모듈
import platform                         # 파이썬 아키텍처를 확인하기 위한 모듈

if 'Windows' == platform.system() :     # 윈도우 운영체제에서 c 모듈 로드
    path = '.\Dll1.dll'
    c_module = ctypes.windll.LoadLibrary(path)
elif 'Linux' == platform.system() :     # 리눅스 운영체제에서 c 모듈 로드
    path = "./libc_module.so"
    c_module = ctypes.cdll.LoadLibrary(path)
else :
    raise OSError()
#함수 관리
add = c_module.add
add.argtypes = (ctypes.c_int, ctypes.c_int)
add.restype = ctypes.c_int

KEY = c_module.KEY
KEY.restype = ctypes.c_int

key = c_module.newkeymaking
key.restype = None

rekey = c_module.rekey
rekey.restype = None

encrypt = c_module.FileEncoding
encrypt.argtype = (ctypes.c_int)
encrypt.restype = ctypes.c_int


insert = c_module.insert
insert.argtype = (ctypes.c_int)
insert.restype = None

decrypt = c_module.filedecrypt
decrypt.restype = None

txtEn = c_module.txtencrypt
txtEn.restype = None

txtDe = c_module.txtdecrypt
txtDe.restype = None

# 키생성
key()

window = TkinterDnD.Tk()
window.configure(bg='azure')

window.title("ENIGMA")
window.geometry("1290x700+300+100")
window.resizable(True, True)

font0 = tkinter.font.Font(size=30, slant="italic", weight="bold")
font1 = tkinter.font.Font(family="Richard Clean Personal Use", size=45)
font2 = tkinter.font.Font(size=15, weight="bold")
font3 = tkinter.font.Font(family="Richard Clean Personal Use", size=10)

label0 = tkinter.Label(window, text="Welcome to", bg='azure',
                       font=font0)
label0.place(x=530, y=10)

label0 = tkinter.Label(window, text="ENIGMA", fg="Green", bg='azure', font=font1)
label0.place(x=520, y=50)

photo = tkinter.PhotoImage(file="C:/Users/robin/PycharmProjects/GUI/key.png")
pLabel = Label(window, image=photo, bg='azure')
pLabel.place(x=760, y=60)

photo1 = tkinter.PhotoImage(file="C:/Users/robin/PycharmProjects/GUI/lock.png")
pLabel1 = Label(window, image=photo1, bg='azure')
pLabel1.place(x=460, y=60)

label1 = tkinter.Label(window, text="Made by OS IX, Project ENIGMA", fg="black", bg='azure', font=font3)
label1.place(x=1100, y=668)

def file_in0(event):
    if address.get() == 'Enter file name or drag it ( File to Encrypt / Decrypt )':
        address.delete(0, "end")  # delete all the text in the entry
        address.insert(0, '')  # Insert blank for user input
        address.config(fg='black')


def file_in1(event):
    if address.get() == '':
        address.insert(0, 'Enter file name or drag it ( File to Encrypt / Decrypt )')
        address.config(fg='black')


def file_in2(event):
        address.delete(0, 'end')  # delete all the text in the entry
        address.config(fg='black')

def drop(event):
    entry_sv.set(event.data)

entry_sv = tkinter.StringVar()
address = tkinter.Entry(window, textvar=entry_sv, width=60,relief="sunken", bd=4,font=('Arial 18'),bg="light slate gray")
address.insert(0, 'Enter file name or drag it ( File to Encrypt / Decrypt )')
address.bind('<FocusIn>', file_in0)
address.bind('<FocusOut>', file_in1)
address.bind('<Enter>' and '<ButtonRelease-1>', file_in2)
address.bind('<<FocusIn>>' and '<ButtonRelease-1>', file_in2)
address.config(fg='black')
address.pack(fill=tkinter.X)
address.place(x=330, y=205)

address.drop_target_register(DND_ALL)
address.dnd_bind('<<Drop>>', drop)

def file_Path0(event):
    if address1.get() == 'Enter Path ( Result will be saved here )':
        address1.delete(0, "end")  # delete all the text in the entry
        address1.insert(0, '')  # Insert blank for user input
        address1.delete(0, 0)
        address1.delete("end", "end")
        address1.config(fg='black')


def file_Path1(event):
    if address1.get() == '':
        address1.insert(0, 'Enter Path ( Result will be saved here )')
        address1.config(fg='black')


address1 = tkinter.Entry(window, width=60, relief="sunken", bd=4, font=('Arial 18'), bg="light slate gray")
address1.insert(0, 'Enter Path ( Result will be saved here )')
address1.bind('<FocusIn>', file_Path0)
address1.bind('<FocusOut>', file_Path1)
address1.place(x=330, y=255)
