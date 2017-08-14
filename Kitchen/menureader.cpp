#include "menureader.h"
#include "appsetting.h"
#include <QThread>
#define MAX_HID_BUF 65
#define MAX_HID_OPEN_TIMEOUT 500
#define MAX_HID_TIMEOUT 3000
#define MAX_BARCODE_PATTERN 512
const char NewLineBreak[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const char NewTagBreak[] = { 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00 };

typedef struct barcodeMapping {
    char character;
    int size;
    uint8_t pattern[MAX_BARCODE_PATTERN];
} barcodeMapping;
const barcodeMapping CharacterMap[] = {
    #if 1
    {'A', 9, {0x20,0x00,0x04,0x00,0x00,0x00,0x00,0x00, 0x20}}, //A
    {'B', 9, {0x20,0x00,0x05,0x00,0x00,0x00,0x00,0x00, 0x20}}, //B
    {'C', 9, {0x20,0x00,0x06,0x00,0x00,0x00,0x00,0x00, 0x20}}, //C
    {'D', 9, {0x20,0x00,0x07,0x00,0x00,0x00,0x00,0x00, 0x20}}, //D
    {'E', 9, {0x20,0x00,0x08,0x00,0x00,0x00,0x00,0x00, 0x20}}, //E
    {'F', 9, {0x20,0x00,0x09,0x00,0x00,0x00,0x00,0x00, 0x20}}, //F
    {'G', 9, {0x20,0x00,0x0A,0x00,0x00,0x00,0x00,0x00, 0x20}}, //G
    {'H', 9, {0x20,0x00,0x0B,0x00,0x00,0x00,0x00,0x00, 0x20}}, //H
    {'I', 9, {0x20,0x00,0x0C,0x00,0x00,0x00,0x00,0x00, 0x20}}, //I
    {'J', 9, {0x20,0x00,0x0D,0x00,0x00,0x00,0x00,0x00, 0x20}}, //J
    {'K', 9, {0x20,0x00,0x0E,0x00,0x00,0x00,0x00,0x00, 0x20}}, //K
    {'L', 9, {0x20,0x00,0x0F,0x00,0x00,0x00,0x00,0x00, 0x20}}, //L
    {'M', 9, {0x20,0x00,0x10,0x00,0x00,0x00,0x00,0x00, 0x20}}, //M
    {'N', 9, {0x20,0x00,0x11,0x00,0x00,0x00,0x00,0x00, 0x20}}, //N
    {'O', 9, {0x20,0x00,0x12,0x00,0x00,0x00,0x00,0x00, 0x20}}, //O
    {'P', 9, {0x20,0x00,0x13,0x00,0x00,0x00,0x00,0x00, 0x20}}, //P
    {'Q', 9, {0x20,0x00,0x14,0x00,0x00,0x00,0x00,0x00, 0x20}}, //Q
    {'R', 9, {0x20,0x00,0x15,0x00,0x00,0x00,0x00,0x00, 0x20}}, //R
    {'S', 9, {0x20,0x00,0x16,0x00,0x00,0x00,0x00,0x00, 0x20}}, //S
    {'T', 9, {0x20,0x00,0x17,0x00,0x00,0x00,0x00,0x00, 0x20}}, //T
    {'U', 9, {0x20,0x00,0x18,0x00,0x00,0x00,0x00,0x00, 0x20}}, //U
    {'W', 9, {0x20,0x00,0x19,0x00,0x00,0x00,0x00,0x00, 0x20}}, //W
    {'V', 9, {0x20,0x00,0x1A,0x00,0x00,0x00,0x00,0x00, 0x20}}, //V
    {'X', 9, {0x20,0x00,0x1B,0x00,0x00,0x00,0x00,0x00, 0x20}}, //X
    {'Y', 9, {0x20,0x00,0x1C,0x00,0x00,0x00,0x00,0x00, 0x20}}, //Y
    {'Z', 9, {0x20,0x00,0x1D,0x00,0x00,0x00,0x00,0x00, 0x20}}, //Z
    #endif

    #if 1
    {'A', 8, {0x02,0x00,0x04,0x00,0x00,0x00,0x00,0x00}}, //A
    {'B', 8, {0x02,0x00,0x05,0x00,0x00,0x00,0x00,0x00}}, //B
    {'C', 8, {0x02,0x00,0x06,0x00,0x00,0x00,0x00,0x00}}, //C
    {'D', 8, {0x02,0x00,0x07,0x00,0x00,0x00,0x00,0x00}}, //D
    {'E', 8, {0x02,0x00,0x08,0x00,0x00,0x00,0x00,0x00}}, //E
    {'F', 8, {0x02,0x00,0x09,0x00,0x00,0x00,0x00,0x00}}, //F
    {'G', 8, {0x02,0x00,0x0A,0x00,0x00,0x00,0x00,0x00}}, //G
    {'H', 8, {0x02,0x00,0x0B,0x00,0x00,0x00,0x00,0x00}}, //H
    {'I', 8, {0x02,0x00,0x0C,0x00,0x00,0x00,0x00,0x00}}, //I
    {'J', 8, {0x02,0x00,0x0D,0x00,0x00,0x00,0x00,0x00}}, //J
    {'K', 8, {0x02,0x00,0x0E,0x00,0x00,0x00,0x00,0x00}}, //K
    {'L', 8, {0x02,0x00,0x0F,0x00,0x00,0x00,0x00,0x00}}, //L
    {'M', 8, {0x02,0x00,0x10,0x00,0x00,0x00,0x00,0x00}}, //M
    {'N', 8, {0x02,0x00,0x11,0x00,0x00,0x00,0x00,0x00}}, //N
    {'O', 8, {0x02,0x00,0x12,0x00,0x00,0x00,0x00,0x00}}, //O
    {'P', 8, {0x02,0x00,0x13,0x00,0x00,0x00,0x00,0x00}}, //P
    {'Q', 8, {0x02,0x00,0x14,0x00,0x00,0x00,0x00,0x00}}, //Q
    {'R', 8, {0x02,0x00,0x15,0x00,0x00,0x00,0x00,0x00}}, //R
    {'S', 8, {0x02,0x00,0x16,0x00,0x00,0x00,0x00,0x00}}, //S
    {'T', 8, {0x02,0x00,0x17,0x00,0x00,0x00,0x00,0x00}}, //T
    {'U', 8, {0x02,0x00,0x18,0x00,0x00,0x00,0x00,0x00}}, //U
    {'W', 8, {0x02,0x00,0x19,0x00,0x00,0x00,0x00,0x00}}, //W
    {'V', 8, {0x02,0x00,0x1A,0x00,0x00,0x00,0x00,0x00}}, //V
    {'X', 8, {0x02,0x00,0x1B,0x00,0x00,0x00,0x00,0x00}}, //X
    {'Y', 8, {0x02,0x00,0x1C,0x00,0x00,0x00,0x00,0x00}}, //Y
    {'Z', 8, {0x02,0x00,0x1D,0x00,0x00,0x00,0x00,0x00}}, //Z
    #endif

    {'0', 8, {0x00,0x00,0x27,0x00,0x00,0x00,0x00, 0x00}},
    {'1', 8, {0x00,0x00,0x1e,0x00,0x00,0x00,0x00, 0x00}},
    {'2', 8, {0x00,0x00,0x1f,0x00,0x00,0x00,0x00, 0x00}},
    {'3', 8, {0x00,0x00,0x20,0x00,0x00,0x00,0x00, 0x00}},
    {'4', 8, {0x00,0x00,0x21,0x00,0x00,0x00,0x00, 0x00}},
    {'5', 8, {0x00,0x00,0x22,0x00,0x00,0x00,0x00, 0x00}},
    {'6', 8, {0x00,0x00,0x23,0x00,0x00,0x00,0x00, 0x00}},
    {'7', 8, {0x00,0x00,0x24,0x00,0x00,0x00,0x00, 0x00}},
    {'8', 8, {0x00,0x00,0x25,0x00,0x00,0x00,0x00, 0x00}},
    {'9', 8, {0x00,0x00,0x26,0x00,0x00,0x00,0x00, 0x00}},
    #if 1
    {'A', 8, {0x00,0x00,0xe5,0x04,0x00,0x00,0x00, 0x00}},
    {'B', 8, {0x00,0x00,0xe5,0x05,0x00,0x00,0x00, 0x00}},
    {'C', 8, {0x00,0x00,0xe5,0x06,0x00,0x00,0x00, 0x00}},
    {'D', 8, {0x00,0x00,0xe5,0x07,0x00,0x00,0x00, 0x00}},
    {'E', 8, {0x00,0x00,0xe5,0x08,0x00,0x00,0x00, 0x00}}, //E
    {'F', 8, {0x00,0x00,0xe5,0x09,0x00,0x00,0x00, 0x00}}, //F
    {'G', 8, {0x00,0x00,0xe5,0x0a,0x00,0x00,0x00, 0x00}}, //G
    {'H', 8, {0x00,0x00,0xe5,0x0b,0x00,0x00,0x00, 0x00}}, //H
    {'I', 8, {0x00,0x00,0xe5,0x0c,0x00,0x00,0x00, 0x00}}, //I
    {'J', 8, {0x00,0x00,0xe5,0x0d,0x00,0x00,0x00, 0x00}}, //J
    {'K', 8, {0x00,0x00,0xe5,0x0e,0x00,0x00,0x00, 0x00}}, //K
    {'L', 8, {0x00,0x00,0xe5,0x0f,0x00,0x00,0x00, 0x00}}, //L
    {'M', 8, {0x00,0x00,0xe5,0x10,0x00,0x00,0x00, 0x00}}, //M
    {'N', 8, {0x00,0x00,0xe5,0x11,0x00,0x00,0x00, 0x00}}, //N
    {'O', 8, {0x00,0x00,0xe5,0x12,0x00,0x00,0x00, 0x00}}, //O
    {'P', 8, {0x00,0x00,0xe5,0x13,0x00,0x00,0x00, 0x00}}, //P
    {'Q', 8, {0x00,0x00,0xe5,0x14,0x00,0x00,0x00, 0x00}}, //Q
    {'R', 8, {0x00,0x00,0xe5,0x15,0x00,0x00,0x00, 0x00}}, //R
    {'S', 8, {0x00,0x00,0xe5,0x16,0x00,0x00,0x00, 0x00}}, //S
    {'T', 8, {0x00,0x00,0xe5,0x17,0x00,0x00,0x00, 0x00}}, //T
    {'U', 8, {0x00,0x00,0xe5,0x18,0x00,0x00,0x00, 0x00}}, //U
    {'W', 8, {0x00,0x00,0xe5,0x19,0x00,0x00,0x00, 0x00}}, //W

    {'V', 8, {0x00,0x00,0xe5,0x1a,0x00,0x00,0x00, 0x00}}, //V
    {'X', 8, {0x00,0x00,0xe5,0x1b,0x00,0x00,0x00, 0x00}}, //X
    {'Y', 8, {0x00,0x00,0xe5,0x1c,0x00,0x00,0x00, 0x00}}, //Y
    {'Z', 8, {0x00,0x00,0xe5,0x1d,0x00,0x00,0x00, 0x00}} //Z
    #endif
};


void MenuReader::read() {
    unsigned short vid = 0x24ea;
    unsigned short pid = 0x0197;
    bool ckhid = false;
    unsigned char buf[MAX_HID_BUF];
    char ch;

    hid_device *handle = NULL;
    std::string tag;
    handle = hid_open(vid, pid, NULL);
    qDebug() << "open success...";
    while (1) {
        ckhid = MenuReader::checkHid(vid, pid);
        while (!ckhid) {
            QThread::sleep(1);
            qDebug() << "try to open...";
            emit statusBarCode(false);
            handle = hid_open(vid, pid, NULL);
            if(handle) {
                break;
            }
        }

        qDebug() << "open...";
        emit statusBarCode(true);
        if(handle) {
            res = hid_read(handle, buf, MAX_HID_BUF);
        }
        if (res > 0) {
            if (memcmp(buf, NewLineBreak, 8) == 0) {
                memset(buf, 0, MAX_HID_BUF);
                continue;
            }
            if (memcmp(buf, NewTagBreak, 8) == 0) {
                std::cout << "Read data: " << tag << std::endl;
                emit orderNumber(QString::fromStdString(tag));
                memset(buf, 0, MAX_HID_BUF);
                tag.clear();
                continue;
            }
            if (MenuReader::mappingChar(buf, res, ch)) {
                tag.push_back(ch);
            } else {
                Utils::print_hex_vector(std::vector<uint8_t>(buf, buf + res));
                std::cout << "Not found" << std::endl;
            }
        }
    }
}

bool MenuReader::checkHid(unsigned short vid, unsigned short pid)
{
    bool isHid = false;
    hid_device *handle = NULL;\
    handle = hid_open(vid, pid, NULL);
    qDebug() << "checkHid";
    if (handle) {
        qDebug() << "result when checkhid";
        isHid = true;
        hid_close(handle);
    }
    return isHid;
}

bool MenuReader::mappingChar(uint8_t *buffer, int len, char &out) {
    int found = -1;
    unsigned int iter = 0;
    for (iter = 0 ; iter < sizeof(CharacterMap) / sizeof(barcodeMapping) && found == -1; iter++){
        if (len == CharacterMap[iter].size) {
            if (memcmp(CharacterMap[iter].pattern, buffer, len) == 0){
                found = 0;
                out = CharacterMap[iter].character;
                return true;
            }
        }
    }
    return false;
}

void MenuReader::timerEvent(QTimerEvent *event) {
    if (event->timerId() == _timerCheckEventHappen) {
        qDebug() << "timerEvent";
    }
}

