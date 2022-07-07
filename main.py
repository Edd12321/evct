#!/usr/bin/env python
 ######################################################
 # This is a GUI wrapper for evct, written in python. #
 ######################################################
import tkinter.filedialog
import tkinter as tk
import os

from tkinter import ttk

# initialize tkinter
root = tk.Tk()
root.title("edd's very cool texteditor")

# screensize variables
vw = root.winfo_screenwidth()
vh = root.winfo_screenheight()
root.geometry("%dx%d+0+0" % (vw, vh))

# tabs
notebook = ttk.Notebook(root)
notebook.pack(expand=1)

top_bar = tk.Menu(root, background="gray52", fg="black")
root.config(menu=top_bar, background="gray")

 ###########################
 # Function initialization #
 ###########################

# open a new tab with another terminal inside of it
def new_tab(command, title):
    # define terminal
    term = tk.Frame(notebook,\
                    width=vw,\
                    height=vh)
    term.pack(fill=tk.BOTH, expand=1)

    # add tab
    notebook.add(term, text='*'+title+'*')

    # get winId
    win_id = term.winfo_id()

    # open terminal inside tab
    os.system("xterm -into "+str(win_id)+\
              " -geometry "+str(vw)+'x'+str(vh)+\
              " -sb -e "+command+\
              " &")

# do nothing
def nop():
    pass

# open file
def fopen():
    filename = tk.filedialog.askopenfilename()
    # open tab
    new_tab(os.getcwd()+"/evct "+filename,\
            filename)
    pass

# compile file
def fexec():
    comm_str = tk.simpledialog.askstring(title="Input Dialog",\
                                         prompt="Which command to run?")
    new_tab(comm_str, "stdout")
    pass

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
root.mainloop()
