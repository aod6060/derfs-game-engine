



fp = open("mouse_buttons.txt", "r")

out = open("output_mousebuttons.txt", "w")

while line := fp.readline():
    l = line.rstrip()
    out.write("{input::MouseButtons::"+l+", \""+l+"\"},\n")

out.close()
fp.close()