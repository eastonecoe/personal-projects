##My group's submission for HackISU 2016. This prototype extracts calendar information from a class syllabus and exports the data to a .cvs file.
##My contributions were the interface and the .cvs exporter.

from tkinter import *
from tkinter import ttk
from tkinter import filedialog
import csv

##def resource_path(relative_path):
##    try:
##        base_path = sys._MEIPASS
##    except Exception:
##        base_path = os.path.abspath(".")
##    return os.path.join(base_path, relative_path)



year = 2016

def getMonths():
    months = []
    #monthfile = resource_path("months.txt")
    infile = open("months.txt", "r")
    for line in infile:
        month = line.strip()
        months.append(month)
    return months

def file2list(infile):
    wordlist = []
    for line in infile:
        line = line.strip()
        line = line.split()
        for entry in line:
            wordlist.append(entry)
    return wordlist

def MonthDayAssignment(year, infile):
    months = getMonths()
    assignments = []
    assignment = []
    month = False
    wordlist = file2list(infile)
    for word in wordlist:
        if word.upper() in months:
            if len(assignment) != 0:
                assignments.append(assignment)
                assignment = []
            assignment.append(word)
            month = True
        elif month:
            try:
                int(word)
                assignment.append(word)
            except:
                assignment=[]
            month = False
        elif len(assignment)>= 2:
            assignment.append(word)
    mdaoutput = ""
    
    for line in assignments:
        if "/" in line[0]: 
            month = line[0][0:1]
            day = line[0][3:4]
            assignment = ""
            for i in range(1, len(line)):
                assignment += line[i]
                assignment += " "
        else:
            for m in range(0,12):
                if months[m] in line[0].upper():
                    month = str(m+1)
            day = line[1]
            assignment = ""
            for i in range(2,len(line)):
                assignment += line[i]
                assignment += " "

        mdaoutput += month
        mdaoutput += '\\'
        mdaoutput += day
        mdaoutput += ': '
        mdaoutput += assignment
        mdaoutput += "\n"
        
    sidetext['state'] = 'normal'
    sidetext.delete('1.0', END)
    sidetext.insert('1.0', mdaoutput)
    sidetext['state'] = 'disabled'
    global globassign
    globassign = assignments

def SlashDateAssignment(year, infile):
    fileList = file2list(infile)
    assignments = []
    assignment = []
    date = False
    slash = "/"
    mdaoutput = ""
    for word in fileList:
        if slash in word:
            d = word.split(slash)
            try:
                int(d[0])
                int(d[1])
                if len(assignment) != 0:
                    assignments.append(assignment)
                    assignment = []
                assignment.append(word)
                if date:
                    mdaoutput += "\n"
                mdaoutput += word
                mdaoutput += ": "
                date = True
            except:
                pass
        elif date:
            assignment.append(word)
            mdaoutput += word
            mdaoutput += " "
    assignments.append(assignment)
    global globassign
    globassign = assignments
    sidetext['state'] = 'normal'
    sidetext.delete('1.0', END)
    sidetext.insert('1.0', mdaoutput)
    sidetext['state'] = 'disabled'

def extract(*args):
    global textinput
    textinput.seek(0,0)
    if datevar.get() == "Month(No.) Day(No.)":
        SlashDateAssignment(year, textinput)
    else:
        MonthDayAssignment(year, textinput)

def loadtext(*args):
    maintext['state'] = 'normal'
    maintext.delete('1.0', END)
    try:
        global textinput
        textinput = open(filedialog.askopenfilename(filetypes = [["Text Document", ".txt"]]), "r", encoding = "utf-8")
        maintext.insert('1.0', textinput.read())
        textinput.seek(0,0)
    except FileNotFoundError:
        maintext.insert('1.0', "File not found")
    maintext['state'] = 'disabled'

def exportcsv(*args):
    months = getMonths()
    try:
        csvfile = open(filedialog.asksaveasfilename(filetypes = [["Comma Separated Values File", ".csv"]], defaultextension = ".csv"), "w")
        cal = csv.writer(csvfile)
        fields = ["Subject","Start Date", "Description"]
        cal.writerow(fields)
        
        global globassign
        assignments = globassign

        for line in assignments:
            if "/" in line[0]:
                date = line[0]+"/"+str(year)
                assignment = ""
                for i in range(1, len(line)):
                    assignment += line[i]
                    assignment += " "
            else:
                for m in range(0,12):
                    if months[m] in line[0].upper():
                        month = str(m+1)
                day = line[1]
                date = month + "/" + day + "/" + str(year)
                assignment = ""
                for i in range(2,len(line)):
                    assignment += line[i]
                    assignment += " "
            if "exam" in assignment.lower():
                subject = "Exam"
            elif "project" in assignment.lower():
                subject = "Project"
            else:
                subject = "Assignment"
            row = [classbox.get() + " " + subject, date, assignment]
            try:
                cal.writerow(row)
            except:
                pass
    except:
        pass

#Window creation
root = Tk()
root.title("Syllabot")
#Logo = resource_path('cal.ico')
root.iconbitmap('cal.ico')
root.option_add('*tearOff', FALSE)
root.rowconfigure(0, weight=1)
root.columnconfigure(0, weight=1)
root.columnconfigure(2, weight=1)


#Document textbox creation
mainframe = ttk.Frame(root)
mainframe.grid(column=0, row=0, sticky=(N,S,E,W))
mainframe.rowconfigure(1, weight=1)
mainframe.columnconfigure(0, weight=1)
mainlabel = ttk.Label(mainframe, text='Raw Text:')
mainlabel.grid(column=0, row=0, sticky=(N))
maintext = Text(mainframe, width=60, height=30)
maintext.grid(column=0, row=1, sticky=(N, W, E, S), padx=[5,0], pady=5)
mainscroll = ttk.Scrollbar(mainframe, orient=VERTICAL, command=maintext.yview)
mainscroll.grid(column=1, row=1, sticky=(N,S), pady=5)
maintext.configure(yscrollcommand=mainscroll.set)
maintext['state'] = 'disabled'

#Divider creation
divider1 = ttk.Separator(root, orient=VERTICAL)
divider1.grid(column=1, row=0, sticky=(N,S), padx=5)
divider2 = ttk.Separator(root, orient=VERTICAL)
divider2.grid(column=3, row=0, sticky=(N,S), padx=5)

#Result textbox creation
sideframe = ttk.Frame(root)
sideframe.grid(column=2, row=0, sticky=(N,S,E,W))
sideframe.columnconfigure(0, weight=1)
sideframe.rowconfigure(1, weight=1)
sidelabel = ttk.Label(sideframe, text='Extracted Schedule:')
sidelabel.grid(column=0, row=0, sticky=(N))
sidetext = Text(sideframe, width=40, height=30)
sidetext.grid(column=0, row=1, sticky=(N, W, E, S), padx=[5,0], pady=[5,0])
sidevscroll = ttk.Scrollbar(sideframe, orient=VERTICAL, command=sidetext.yview)
sidevscroll.grid(column=1, row=1, sticky=(N,S), pady=[5,0])
sidehscroll = ttk.Scrollbar(sideframe, orient=HORIZONTAL, command=sidetext.xview)
sidehscroll.grid(column=0, row=2, sticky=(N,E,W), padx=[5,0])
sidetext.configure(yscrollcommand=sidevscroll.set)
sidetext.configure(xscrollcommand=sidehscroll.set)
sidetext['state'] = 'disabled'
sidetext['wrap'] = 'none'

#Date Format Section
dateframe = ttk.Frame(root)
dateframe.grid(column=4, row=0, sticky=(N,S,E,W))
datelabel = ttk.Label(dateframe, text='Date Format:')
datelabel.grid(column=0, row=2, sticky=(N,W), padx=5)
datevar = StringVar()
datemenu = ttk.Combobox(dateframe, textvariable=datevar)
datevar.set('Month(Name) Day(No.)')
datemenu.grid(column=0, row=3, sticky=(N,W), padx=5, pady=5)
datemenu['values'] = ['Month(Name) Day(No.)', 'Month(No.) Day(No.)']
datemenu['state'] = 'readonly'
datebutton = ttk.Button(dateframe, command=extract, text="Extract")
datebutton.grid(column=0, row=4)

#Class Name Field
classlabel = ttk.Label(dateframe, text="Name of Class:")
classlabel.grid(column=0, row=0)
classvar = StringVar()
classbox = ttk.Entry(dateframe, textvariable=classvar)
classbox.grid(column=0, row=1, sticky=(E,W), padx=5, pady=5)



#Menu management
menubar = Menu(root)
root['menu'] = menubar
menu_file = Menu(menubar)
menubar.add_cascade(menu=menu_file, label='File')
menu_file.add_command(label='Open...', command=loadtext)
menu_file.add_command(label='Export as .csv...', command=exportcsv)

root.mainloop()
