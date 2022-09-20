"""
-------------------------
How to use this converter
-------------------------
python convert_obj_three.py -i infile.obj -o outfile.txt 
Notes:
    - flags
        -i infile.obj			input OBJ file
        -o outfile.js			output txt file

"""

import fileinput


# #####################################################
# Utils
# #####################################################
def file_exists(filename):
    """Return true if file exists and is accessible for reading.
    Should be safer than just testing for existence due to links and
    permissions magic on Unix filesystems.
    @rtype: boolean
    """

    try:
        f = open(filename, 'r')
        f.close()
        return True
    except IOError:
        return False



# #####################################################
# OBJ parser
# #####################################################
def parse_vertex(text):
    """Parse text chunk specifying single vertex.
    Possible formats:
        vertex index
        vertex index / texture index
        vertex index / texture index / normal index
        vertex index / / normal index
    """

    v = 0
    t = 0
    n = 0

    chunks = text.split("/")

    v = int(chunks[0])
    if len(chunks) > 1:
        if chunks[1]:
            t = int(chunks[1])
    if len(chunks) > 2:
        if chunks[2]:
            n = int(chunks[2])

    return { 'v':v, 't':t, 'n':n }

def parse_obj(fname):
    """Parse OBJ file.
    """

    vertices = []
    normals = []
    uvs = []

    faces = []


    triangles = []

    materials = {}
    material = ""
    mcounter = 0
    mcurrent = 0

    mtllib = ""

    # current face state
    group = 0
    object = 0
    smooth = 0

    for line in fileinput.input(fname):
        chunks = line.split()
        if len(chunks) > 0:

            # Vertices as (x,y,z) coordinates
            # v 0.123 0.234 0.345
            if chunks[0] == "v" and len(chunks) == 4:
                x = float(chunks[1])
                y = float(chunks[2])
                z = float(chunks[3])
                vertices.append([x,y,z])

            # Normals in (x,y,z) form; normals might not be unit
            # vn 0.707 0.000 0.707
            if chunks[0] == "vn" and len(chunks) == 4:
                x = float(chunks[1])
                y = float(chunks[2])
                z = float(chunks[3])
                normals.append([x,y,z])

            # Texture coordinates in (u,v[,w]) coordinates, w is optional
            # vt 0.500 -1.352 [0.234]
            if chunks[0] == "vt" and len(chunks) >= 3:
                u = float(chunks[1])
                v = float(chunks[2])
                w = 0
                if len(chunks)>3:
                    w = float(chunks[3])
                uvs.append([u,v,w])

            # Face
            if chunks[0] == "f" and len(chunks) >= 4:
                vertex_index = []
                uv_index = []
                normal_index = []

                triangles.append(chunks[1])
                triangles.append(chunks[2])
                triangles.append(chunks[3])
                # Precompute vert / normal / uv lists
                # for negative index lookup
                vertlen = len(vertices) + 1
                normlen = len(normals) + 1
                uvlen = len(uvs) + 1

                for v in chunks[1:]:
                    vertex = parse_vertex(v)
                    if vertex['v']:
                        if vertex['v'] < 0:
                            vertex['v'] += vertlen
                        vertex_index.append(vertex['v'])
                    if vertex['t']:
                        if vertex['t'] < 0:
                            vertex['t'] += uvlen
                        uv_index.append(vertex['t'])
                    if vertex['n']:
                        if vertex['n'] < 0:
                            vertex['n'] += normlen
                        normal_index.append(vertex['n'])
                faces.append({
                    'vertex':vertex_index,
                    'uv':uv_index,
                    'normal':normal_index,

                    'material':mcurrent,
                    'group':group,
                    'object':object,
                    'smooth':smooth,
                    })

            # Group
            if chunks[0] == "g" and len(chunks) == 2:
                group = chunks[1]

            # Object
            if chunks[0] == "o" and len(chunks) == 2:
                object = chunks[1]

            # Materials definition
            if chunks[0] == "mtllib" and len(chunks) == 2:
                mtllib = chunks[1]

            # Material
            if chunks[0] == "usemtl":
                if len(chunks) > 1:
                    material = chunks[1]
                else:
                    material = ""
                if not material in materials:
                    mcurrent = mcounter
                    materials[material] = mcounter
                    mcounter += 1
                else:
                    mcurrent = materials[material]

            # Smooth shading
            if chunks[0] == "s" and len(chunks) == 2:
                smooth = chunks[1]

    return vertices, triangles, faces,  uvs, normals, materials, mtllib



