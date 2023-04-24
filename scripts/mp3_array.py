# %% open the file in.mp3 for reading
with open('in.mp3', 'rb') as fi:
    # read the file into a byte array
    b = bytearray(fi.read())

# %% open the file out.h for writing
with open('out.h', 'w') as fo:
    # write the byte array in the form of a c array definition
    fo.write('#include <cstdint>\n')
    fo.write('const uint8_t mp3_data[] = {\n')
    for i in range(len(b)):
        fo.write('0x{:02x}'.format(b[i]))
        if i < len(b) - 1:
            fo.write(',')
        if i % 10 == 0 and i > 0:
            fo.write('\n')
    fo.write('};')