
import getopt
import sys
import os


import parse_obj
import navmesh

# #############################################################################
# Helpers
# #############################################################################
def usage():
    print("Usage: {} -i filename.obj -o filename.js".format(os.path.basename(sys.argv[0])))




def get_obj_info(infile):
    if not parse_obj.file_exists(infile):
        print("Couldn't find [%s]" % infile)
        return

    vertices, triangles, _, _, _,_,_  = parse_obj.parse_obj(infile)

    return vertices, triangles




# #####################################################
# Main
# #####################################################
if __name__ == "__main__":

    # get parameters from the command line
    try:
        opts, args = getopt.getopt(sys.argv[1:], "hi:o:", ["help", "input=",  "output="])

    except getopt.GetoptError:
        usage()
        sys.exit(2)

    infile = outfile = ""

    for o, a in opts:
        if o in ("-h", "--help"):
            usage()
            sys.exit()

        elif o in ("-i", "--input"):
            infile = a

        elif o in ("-o", "--output"):
            outfile = a

    if infile == "" or outfile == "":
        usage()
        sys.exit(2)

    vertices, triangles = get_obj_info(infile)
    mesh = navmesh.NavMash(vertices, triangles)
    mesh.gen_navmesh_file(outfile)
