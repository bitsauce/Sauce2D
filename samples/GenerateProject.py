from shutil import *
import os

project_name = input("Project name: ")

sln_file = project_name + "/" + project_name + ".sln"
proj_file = project_name + "/Project/" + project_name + ".vcxproj"
maincpp_file = project_name + "/Source/Main.cpp"

os.makedirs(project_name)
os.makedirs(project_name + "/Project")
os.makedirs(project_name + "/Source")

copyfile("Minimal/Minimal.sln", project_name + "/" + project_name + ".sln")
copyfile("Minimal/Project/Minimal.vcxproj", project_name + "/Project/" + project_name + ".vcxproj")
copyfile("Minimal/Source/Main.cpp", project_name + "/Source/Main.cpp")

def replaceInFiles(file, old, new):
    with open(file, "r+") as f:
        lines = f.readlines()
        for i in range(len(lines)):
            lines[i] = lines[i].replace(old, new)
        f.seek(0);
        f.writelines(lines)
        f.truncate()

replaceInFiles(sln_file, "Minimal", project_name)
replaceInFiles(proj_file, "Minimal", project_name)
replaceInFiles(maincpp_file, "Minimal", project_name)