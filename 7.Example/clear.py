import os
import shutil

DIR_PREFIX = []
DIR_SUFFIX = []

FILE_PREFIX = ["startup_"]
FILE_SUFFIX = [".uvprojx", ".uvoptx", ".hex"]


def check(name, prefixs, suffixs):
    for prefix in prefixs:
        if name.startswith(prefix):
            return True
    for suffix in suffixs:
        if name.endswith(suffix):
            return True
    return False


def clear(dirpath):
    for name in os.listdir(dirpath):
        _ = os.path.join(dirpath, name)
        if os.path.isdir(_):
            if check(name, DIR_PREFIX, DIR_SUFFIX):
                continue
            # if dir not in white list, remove it
            clear(_)
        elif os.path.isfile(_):
            if check(name, FILE_PREFIX, FILE_SUFFIX):
                continue
            # if file not in white list, remove it
            os.remove(_)
    # if current dir is null, remove it
    if len(os.listdir(dirpath)) == 0:
        shutil.rmtree(dirpath)


for root, dirs, files in os.walk('.'):
    for dir in dirs:
        if dir == "MDK-ARM":
            clear(os.path.join(root, dir))
