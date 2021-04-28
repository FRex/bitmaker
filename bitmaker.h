typedef struct bitmaker {
    int count;
    unsigned char buff;
    unsigned char * bytes;
    int bytecount;
    int bytecapacity;
} bitmaker;

void bitmaker_init(bitmaker * b);

int bitmaker_addBit(bitmaker * b, int v);
void bitmaker_addBigBytes(bitmaker * b, int size, unsigned value);
void bitmaker_padToByte(bitmaker * b);

void bitmaker_setBitAtAnchor(bitmaker * b, int anchor, int v);
//TODO: delete all past the anchor
//TODO: make add big bytes return anchor to front too
