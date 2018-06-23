from shutil import *
import os
import argparse

source_dir = os.path.join("samples", "Minimal")

# Parse command-line arguments
parser = argparse.ArgumentParser()
parser.add_argument("project_name", help="Project name")
parser.add_argument("--target_dir", default="samples", help="Target directory for project files")
args = parser.parse_args()

# Setup all target paths
target_dir    = os.path.join(args.target_dir, args.project_name)
if os.path.isdir(target_dir):
    raise Exception("A project named {} already exists. Please remove the existsing project or choose a different name".format(args.project_name))

sln_file      = os.path.join(target_dir, args.project_name + ".sln")
proj_file     = os.path.join(target_dir, "Project", args.project_name + ".vcxproj")
maincpp_file  = os.path.join(target_dir, "Source", "Main.cpp")
rc_file       = os.path.join(target_dir, "Data", "Resources.xml")
font_file     = os.path.join(target_dir, "Data", "Arial.fnt")
font_img_file = os.path.join(target_dir, "Data", "Arial_0.png")

# Make target directories
os.makedirs(target_dir)
os.makedirs(os.path.join(target_dir, "Project"))
os.makedirs(os.path.join(target_dir, "Source"))
os.makedirs(os.path.join(target_dir, "Data"))

# Copy files from "Minimal" into target directory
copyfile(os.path.join(source_dir, "Minimal.sln"), sln_file)
copyfile(os.path.join(source_dir, "Project", "Minimal.vcxproj"), proj_file)
copyfile(os.path.join(source_dir, "Source", "Main.cpp"), maincpp_file)
copyfile(os.path.join(source_dir, "Data", "Resources.xml"), rc_file)
copyfile(os.path.join(source_dir, "Data", "Arial.fnt"), font_file)
copyfile(os.path.join(source_dir, "Data", "Arial_0.png"), font_img_file)

def replaceInFiles(files, old, new):
    for file in files:
        with open(file, "r+", encoding="utf8") as f:
            lines = f.readlines()
            for i in range(len(lines)):
                lines[i] = lines[i].replace(old, new)
            f.seek(0)
            f.writelines(lines)
            f.truncate()

replaceInFiles([sln_file, proj_file, maincpp_file], "Minimal", args.project_name)