#!/usr/bin/env python
 ######################################################
 # This is a GUI wrapper for evct, written in python. #
 ######################################################
 # It currently only works properly on UNIX/with Cygwin
import tkinter.filedialog
import tkinter as tk
import os
import subprocess
import sys

from tkinter import ttk
from pathlib import Path

root_bg = "#ddd"
bar_bg  = "white"
bar_fg  = "black"
win_bg  = "white"
win_fg  = "black"

# initialize tkinter
root = tk.Tk()
root.title("Edd's Very Cool Texteditor")

# screensize variables
vw = root.winfo_screenwidth()  >> 1
vh = root.winfo_screenheight() >> 1

root.geometry("{}x{}+{}+{}".format(vw, vh, vw, vh))

# tabs
notebook = ttk.Notebook(root)
notebook.pack(expand=1)

top_bar = tk.Menu(root, background=bar_bg, fg=bar_fg)
root.config(menu=top_bar, background=root_bg)

# path/env variables
script_path = os.path.abspath(sys.argv[0])
script_path = os.path.abspath(os.path.join(script_path, os.pardir))

 ###########################
 # Function initialization #
 ###########################

# open a new tab with another terminal inside of it
def new_tab(command, title):
    # define terminal
    term = tk.Frame(notebook,\
                    width=vw,\
                    height=vh)
    term.pack(fill="both", expand=1)

    # add tab
    notebook.add(term, text='*'+title+'*')

    # get winId
    win_id = term.winfo_id()

    # open terminal inside tab
    try:
        command_str = "xterm -into "+str(win_id)+\
                    " -geometry "+str(vw)+'x'+str(vh)+\
                    " -hold"+\
                    " -e "+command+" &"
    except FileNotFoundError:
        tk.showwarning("Error", "xterm not installed !")

    if sys.platform == "win32":
        os.system("sh -c \"mount -a;export DISPLAY=:0;"+command_str+"\"")
    else:
        os.system("export DISPLAY=:0;"+command_str)

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

    script_path += "/evct"
    # add extension
    if sys.platform == "cygwin" or sys.platform == "win32":
        script_path = script_path+".exe"

    new_tab(script_path+' '+filename,\
            filename)

# compile file
def fexec():
    comm_str = tk.simpledialog.askstring(title="Input Dialog",\
                                         prompt="Which command to run?")
    new_tab(comm_str, "stdout")

# close evct
def kill():
    exit()

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

# start tk
root.update()
root.mainloop()
