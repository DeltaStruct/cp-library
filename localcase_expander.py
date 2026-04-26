from pathlib import Path
import shutil
import sys

def expand_localcase(path,in_s,out_s):
  path = Path("deltastruct/verify/localcase/"+path)
  for s in [in_s,out_s]:
    src = path/s
    if not src.exists():
      print("Path",src,"not found.")
    for f in src.iterdir():
      if f.is_file():
        shutil.copy2(f,path/(f.name+"."+s))
        print(f"Copied {f} -> {path/(f.name+"."+s)}")

if __name__=="__main__":
  expand_localcase(sys.argv[1],sys.argv[2] if len(sys.argv)>3 else "in",sys.argv[3] if len(sys.argv)>3 else "out")