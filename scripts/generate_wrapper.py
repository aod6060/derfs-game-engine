



fp = open("mouse_buttons.txt", "r")

out = open("output_config_mousebuttons.txt", "w")

while line := fp.readline():
    l = line.rstrip()
    #out.write("{input::MouseButtons::"+l+", \""+l+"\"},\n")
    out.write("{\""+l+"\", input::MouseButtons::"+l+"},\n")

out.close()
fp.close()