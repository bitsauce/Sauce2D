from shutil import *
import os

project_name = input("Project name: ").strip();

sln_file = project_name + "/" + project_name + ".sln"
proj_file = project_name + "/Project/" + project_name + ".vcxproj"
maincpp_file = project_name + "/Source/Main.cpp"
rc_file = project_name + "/Data/Resources.xml"
font_file = project_name + "/Data/Arial.fnt"
font_img_file = project_name + "/Data/Arial_0.png"

os.makedirs(project_name)
os.makedirs(project_name + "/Project")
os.makedirs(project_name + "/Source")
os.makedirs(project_name + "/Data")

dir_path = os.path.dirname(os.path.realpath(__file__))
copyfile(dir_path + "/samples/Minimal/Minimal.sln", sln_file)
copyfile(dir_path + "/samples/Minimal/Project/Minimal.vcxproj", proj_file)
copyfile(dir_path + "/samples/Minimal/Source/Main.cpp", maincpp_file)
copyfile(dir_path + "/samples/Minimal/Data/Resources.xml", rc_file)
copyfile(dir_path + "/samples/Minimal/Data/Arial.fnt", font_file)
copyfile(dir_path + "/samples/Minimal/Data/Arial_0.png", font_img_file)

def replaceInFiles(files, old, new):
    for file in files:
        with open(file, "r+") as f:
            lines = f.readlines()
            for i in range(len(lines)):
                lines[i] = lines[i].replace(old, new)
            f.seek(0)
            f.writelines(lines)
            f.truncate()

replaceInFiles([sln_file, proj_file, maincpp_file, rc_file, font_file, font_img_file], "Minimal", project_name)