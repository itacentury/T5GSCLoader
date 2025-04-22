import re
log = open("t5.log", "r").read()
lines = re.findall(r"Asset '([^']+\.gsc)' with buffer '0x([0-9A-Fa-f]+)'", log)
print("static const struct { const char *name; uintptr_t buf; } builtinGSCs[] = {")
for name, addr in lines:
    print(f"    {{ \"{name}\", (uintptr_t)0x{addr} }},")
print("};")
