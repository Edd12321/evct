#!/usr/bin/env python
 ######################################################
 # This is a GUI wrapper for evct, written in python. #
 ######################################################
 # It currently only works properly on UNIX/with Cygwin
import tkinter.filedialog
import tkinter as tk
import os
import sys
import argparse
import subprocess
import signal

from tkinter import ttk
from pathlib import Path

# allow window resize
os.system("echo '*VT100.allowWindowOps: true' | xrdb -merge")

# get cli arguments
## X11 display number (default :0)
parser = argparse.ArgumentParser(description="evct gui wrapper",
                                 formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument("-x", "--x11dp",
                    type=str,
                    help="X11 Display Number",
                    default=":0")
parser.add_argument("-t", "--vterm",
                    type=str,
                    help="Terminal with WinID support",
                    default="xterm -into")

args = parser.parse_args()

statement_ran = 0

# initialize tkinter
root = tk.Tk()
root.title("Edd's Very Cool Texteditor")

##THEME
if sys.platform != "win32":
    style = ttk.Style(root)
    style.theme_use("alt")
    style.configure("TFrame",
                    background="#fff",
                    foreground="#000")

# screensize variables
vw = root.winfo_screenwidth()  >> 1
vh = root.winfo_screenheight() >> 1

root.geometry("{}x{}+{}+{}".format(vw, vh, vw, vh))

# tabs
notebook = ttk.Notebook(root)
notebook.pack(expand=1)

top_bar = tk.Menu(root)
root.config(menu=top_bar)

# path/env variables
script_path = os.path.abspath(sys.argv[0])
script_path = os.path.abspath(os.path.join(script_path, os.pardir))

if statement_ran == 0:
    statement_ran = 1
    script_path += "/evct"
    # add extension
    if sys.platform == "cygwin" or sys.platform == "win32":
        script_path = script_path+".exe"

 ###########################
 # Function initialization #
 ###########################

# open a new tab with another terminal inside of it
def new_tab(command, title):
    global pid
    # define terminal
    term = tk.Frame(notebook,
                    width=vw,
                    height=vh)
    term.pack(fill="both", expand=1)

    # add tab
    notebook.add(term, text='*'+title+'*')

    # get winId
    win_id = term.winfo_id()

    # open terminal inside tab
    try:
        # don't ask
        command_str = f'{args.vterm} {str(win_id)}'\
                     +f' -geometry {str(vw)}x{str(vh)}'\
                     +f' -sb -e " echo -e \'\\e\\[4;{str(vh-55)};{str(vw)}t\';'\
                     +command+'" &'

    except FileNotFoundError:
        tk.showwarning("Error", f'{args.vterm} not available !')

    os.environ["DISPLAY"] = args.x11dp
    if sys.platform == "win32":
        subprocess.Popen("sh -c \"mount -a;"+command_str+"\"",
                         shell=1)
    else:
        subprocess.Popen(command_str, shell=1)

# do nothing
def nop():
    pass

# open file
def fopen():
    global filename, script_path

    filename = tk.filedialog.askopenfilename()
    # open tab
    if sys.platform == "win32":
        # because windows likes to be different, we have to change the paths formatting
        drive_letter_fn = filename[0].lower()
        filename = filename.replace("\\", '/')
        filename = filename[2:]
        filename = "/cygdrive/"+drive_letter_fn+filename

        drive_letter_sc = script_path[0].lower()
        script_path = script_path.replace("\\", '/')
        script_path = script_path[2:]
        script_path = "/cygdrive/"+drive_letter_sc+script_path

    new_tab(script_path+' '+filename,\
            filename)

# compile file
def fexec():
    comm_str = tk.simpledialog.askstring(title="Input Dialog",
                                         prompt="Which command to run?",)
    new_tab(comm_str, comm_str)

# update size
def conf(event):
    global vw, vh
    vw = root.winfo_width()
    vh = root.winfo_height()

    notebook.config(height=vh,\
                    width=vw)

# close evct
def kill():
    os.killpg(os.getpid(), signal.SIGTERM)

def close_seltab():
    notebook.forget("current")

 ###############################
 # Add navigation bar elements #
 ###############################

## File
files_select = tk.Menu(top_bar, tearoff=0)
top_bar.add_cascade(label="File", menu=files_select)
files_select.add_command(label="New/Open", command=fopen)
files_select.add_command(label="Close", command=kill)

## Build
build_select = tk.Menu(top_bar, tearoff=0)
top_bar.add_cascade(label="Build", menu=build_select)
build_select.add_command(label="Execute/Compile", command=fexec)

## Tabs
tabs_select  = tk.Menu(top_bar, tearoff=0)
top_bar.add_cascade(labe="Tabs", menu=tabs_select)
tabs_select.add_command(label="Close selected", command=close_seltab)

# start tk
root.bind("<Configure>", conf)
root.mainloop()
