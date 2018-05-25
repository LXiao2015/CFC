infile = "raw.txt"
outfile = "flow.txt"

f = open(outfile, "a+")
f.seek(0)       # 定位到文件头
f.truncate()    # 清空文件

line_count = 1
for line in open(infile):
	if line_count != 1:
		val = line.split()
		line = ", ".join(val)
		line = "    [" + line + "],\n"
		f.write(line)
	line_count += 1