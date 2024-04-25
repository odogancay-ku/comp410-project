//
// Created by ofarukdogancay on 4/25/24.
//

#include "SegmentDisplay.h"
#include <bitset>

std::map<int, char *> segmentNames = {
        {0,  "g1"},
        {1,  "k"},
        {2,  "l"},
        {3,  "m"},
        {4,  "g2"},
        {5,  "j"},
        {6,  "i"},
        {7,  "h"},
        {8,  "f"},
        {9,  "e"},
        {10, "d1"},
        {11, "d2"},
        {12, "c"},
        {13, "b"},
        {14, "a2"},
        {15, "a1"},
};

// M 0b0000010111001100

const uint16_t SixteenSegmentASCII[96] = {
        0b0000000000000000, /* (space) */
        0b0000000000001100, /* ! */
        0b0000001000000100, /* " */
        0b1010101000111100, /* # */
        0b1010101010111011, /* $ */
        0b1110111010011001, /* % */
        0b1001001101110001, /* & */
        0b0000001000000000, /* ' */
        0b0001010000000000, /* ( */
        0b0100000100000000, /* ) */
        0b1111111100000000, /* * */
        0b1010101000000000, /* + */
        0b0100000000000000, /* , */
        0b1000100000000000, /* - */
        0b0001000000000000, /* . */
        0b0100010000000000, /* / */
        0b0100010011111111, /* 0 */
        0b0000010000001100, /* 1 */
        0b1000100001110111, /* 2 */
        0b0000100000111111, /* 3 */
        0b1000100010001100, /* 4 */
        0b1001000010110011, /* 5 */
        0b1000100011111011, /* 6 */
        0b0000000000001111, /* 7 */
        0b1000100011111111, /* 8 */
        0b1000100010111111, /* 9 */
        0b0010001000000000, /* : */
        0b0100001000000000, /* ; */
        0b1001010000000000, /* < */
        0b1000100000110000, /* = */
        0b0100100100000000, /* > */
        0b0010100000000111, /* ? */
        0b0000101011110111, /* @ */
        0b1000100011001111, /* A */
        0b0010101000111111, /* B */
        0b0000000011110011, /* C */
        0b0010001000111111, /* D */
        0b1000000011110011, /* E */
        0b1000000011000011, /* F */
        0b0000100011111011, /* G */
        0b1000100011001100, /* H */
        0b0010001000110011, /* I */
        0b0000000001111100, /* J */
        0b1001010011000000, /* K */
        0b0000000011110000, /* L */
        0b0000010111001100, /* M */
        0b0001000111001100, /* N */
        0b0000000011111111, /* O */
        0b1000100011000111, /* P */
        0b0001000011111111, /* Q */
        0b1001100011000111, /* R */
        0b1000100010111011, /* S */
        0b0010001000000011, /* T */
        0b0000000011111100, /* U */
        0b0100010011000000, /* V */
        0b0101000011001100, /* W */
        0b0101010100000000, /* X */
        0b1000100010111100, /* Y */
        0b0100010000110011, /* Z */
        0b0010001000010010, /* [ */
        0b0001000100000000, /* \ */
        0b0010001000100001, /* ] */
        0b0101000000000000, /* ^ */
        0b0000000000110000, /* _ */
        0b0000000100000000, /* ` */
        0b1010000001110000, /* a */
        0b1010000011100000, /* b */
        0b1000000001100000, /* c */
        0b0010100000011100, /* d */
        0b1100000001100000, /* e */
        0b1010101000000010, /* f */
        0b1010001010100001, /* g */
        0b1010000011000000, /* h */
        0b0010000000000000, /* i */
        0b0010001001100000, /* j */
        0b0011011000000000, /* k */
        0b0000000011000000, /* l */
        0b1010100001001000, /* m */
        0b1010000001000000, /* n */
        0b1010000001100000, /* o */
        0b1000001011000001, /* p */
        0b1010001010000001, /* q */
        0b1000000001000000, /* r */
        0b1010000010100001, /* s */
        0b1000000011100000, /* t */
        0b0010000001100000, /* u */
        0b0100000001000000, /* v */
        0b0101000001001000, /* w */
        0b0101010100000000, /* x */
        0b0000101000011100, /* y */
        0b1100000000100000, /* z */
        0b1010001000010010, /* { */
        0b0010001000000000, /* | */
        0b0010101000100001, /* } */
        0b1100110000000000, /* ~ */
        0b0000000000000000, /* (del) */
};

SegmentDisplay::SegmentDisplay(glm::vec3 position, glm::quat orientation, float scale) {

    this->position = position;
    this->orientation = orientation;
    this->scale = scale;
    this->segmentWidth = 0.3f * scale;

    segments["a1"] = new Object();
    segments["a1"]->canMove = false;
    segments["a1"]->canCollide = false;
    segments["a1"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["a1"]->stretch = glm::vec3(segmentWidth, segmentWidth, 0.75f * scale);
    localOrientations["a1"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["a1"] = glm::vec3(0.0f, scale * 2, -scale / 2);

    segments["a2"] = new Object();
    segments["a2"]->canMove = false;
    segments["a2"]->canCollide = false;
    segments["a2"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["a2"]->stretch = glm::vec3(segmentWidth, segmentWidth, 0.75f * scale);
    localOrientations["a2"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["a2"] = glm::vec3(0.0f, scale * 2, scale / 2);

    segments["f"] = new Object();
    segments["f"]->canMove = false;
    segments["f"]->canCollide = false;
    segments["f"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["f"]->stretch = glm::vec3(segmentWidth, 1.75f * scale, segmentWidth);
    localOrientations["f"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["f"] = glm::vec3(0.0f, scale, -scale);

    segments["b"] = new Object();
    segments["b"]->canMove = false;
    segments["b"]->canCollide = false;
    segments["b"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["b"]->stretch = glm::vec3(segmentWidth, 1.75f * scale, segmentWidth);
    localOrientations["b"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["b"] = glm::vec3(0.0f, scale, scale);

    segments["h"] = new Object();
    segments["h"]->canMove = false;
    segments["h"]->canCollide = false;
    segments["h"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["h"]->stretch = glm::vec3(segmentWidth, scale * 1.5f, segmentWidth);
    localOrientations["h"] = glm::angleAxis(glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    localPositions["h"] = glm::vec3(0.0f, scale, -scale / 2);

    segments["j"] = new Object();
    segments["j"]->canMove = false;
    segments["j"]->canCollide = false;
    segments["j"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["j"]->stretch = glm::vec3(segmentWidth, scale * 1.5f, segmentWidth);
    localOrientations["j"] = glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    localPositions["j"] = glm::vec3(0.0f, scale, scale / 2);

    segments["i"] = new Object();
    segments["i"]->canMove = false;
    segments["i"]->canCollide = false;
    segments["i"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["i"]->stretch = glm::vec3(segmentWidth, scale * 1.75f, segmentWidth);
    localOrientations["i"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["i"] = glm::vec3(0.0f, scale, 0.0f);

    segments["g1"] = new Object();
    segments["g1"]->canMove = false;
    segments["g1"]->canCollide = false;
    segments["g1"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["g1"]->stretch = glm::vec3(segmentWidth, segmentWidth, 0.75f * scale);
    localOrientations["g1"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["g1"] = glm::vec3(0.0f, 0.0f, -scale / 2);

    segments["g2"] = new Object();
    segments["g2"]->canMove = false;
    segments["g2"]->canCollide = false;
    segments["g2"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["g2"]->stretch = glm::vec3(segmentWidth, segmentWidth, 0.75f * scale);
    localOrientations["g2"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["g2"] = glm::vec3(0.0f, 0.0f, scale / 2);

    // f with mirrored y coordinates
    segments["e"] = new Object();
    segments["e"]->canMove = false;
    segments["e"]->canCollide = false;
    segments["e"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["e"]->stretch = glm::vec3(segmentWidth, 1.75f * scale, segmentWidth);
    localOrientations["e"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["e"] = glm::vec3(0.0f, -scale, -scale);


    // b with mirrored y coordinates
    segments["c"] = new Object();
    segments["c"]->canMove = false;
    segments["c"]->canCollide = false;
    segments["c"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["c"]->stretch = glm::vec3(segmentWidth, 1.75f * scale, segmentWidth);
    localOrientations["c"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["c"] = glm::vec3(0.0f, -scale, scale);

    // h with mirrored y coordinates
    segments["k"] = new Object();
    segments["k"]->canMove = false;
    segments["k"]->canCollide = false;
    segments["k"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["k"]->stretch = glm::vec3(segmentWidth, scale * 1.5f, segmentWidth);
    localOrientations["k"] = glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    localPositions["k"] = glm::vec3(0.0f, -scale, -scale / 2);

    // j with mirrored y coordinates
    segments["m"] = new Object();
    segments["m"]->canMove = false;
    segments["m"]->canCollide = false;
    segments["m"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["m"]->stretch = glm::vec3(segmentWidth, scale * 1.5f, segmentWidth);
    localOrientations["m"] = glm::angleAxis(glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    localPositions["m"] = glm::vec3(0.0f, -scale, scale / 2);

    // i with mirrored y coordinates
    segments["l"] = new Object();
    segments["l"]->canMove = false;
    segments["l"]->canCollide = false;
    segments["l"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["l"]->stretch = glm::vec3(segmentWidth, scale * 1.75f, segmentWidth);
    localOrientations["l"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["l"] = glm::vec3(0.0f, -scale, 0.0f);


    // g1 with mirrored y coordinates
    segments["d1"] = new Object();
    segments["d1"]->canMove = false;
    segments["d1"]->canCollide = false;
    segments["d1"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["d1"]->stretch = glm::vec3(segmentWidth, segmentWidth, 0.75f * scale);
    localOrientations["d1"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["d1"] = glm::vec3(0.0f, -1.75f * scale, -scale / 2);

    // g2 with mirrored y coordinates
    segments["d2"] = new Object();
    segments["d2"]->canMove = false;
    segments["d2"]->canCollide = false;
    segments["d2"]->color = glm::vec3(1.0f, 0.0f, 0.0f);
    segments["d2"]->stretch = glm::vec3(segmentWidth, segmentWidth, 0.75f * scale);
    localOrientations["d2"] = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    localPositions["d2"] = glm::vec3(0.0f, -1.75f * scale, scale / 2);

    std::cout << "Segment Display created" << std::endl;

}

void SegmentDisplay::lightSegment(char *segmentName) {

    segments[segmentName]->isHidden = false;

}

void SegmentDisplay::unlightSegment(char *segmentName) {

    segments[segmentName]->isHidden = true;

}

void SegmentDisplay::updateSegments() {

    for (auto segment: segments) {
        segment.second->position = position + orientation * localPositions[segment.first];
        segment.second->orientation = orientation * localOrientations[segment.first];
    }

}

std::vector<Object *> SegmentDisplay::getAllSegmentsAsVector() {

    std::vector<Object *> v;

    for (auto segment: segments) {
        v.push_back(segment.second);
    }

    return v;
}

void SegmentDisplay::displayCharacter(char c) {

    char *segmentName;

    uint16_t mask = 0b1000000000000000;

    for (int i = 0; i < 16; i++) {
//        std::cout << "i: " << i << std::endl;
//        std::bitset<16> maskBitset(mask);
//        std::cout << "Character: " << c << std::endl;
//        std::cout << "Character - 32: " << c - 32 << std::endl;
//        std::bitset<16> segmentBitset(SixteenSegmentASCII[c - 32]);
//        std::cout << "Mask: \n" << maskBitset << std::endl;
//        std::cout << "SixteenSegmentASCII[c - 32]: \n" << segmentBitset << std::endl;
//        std::cout << "SixteenSegmentASCII[c - 32] & mask: " << (SixteenSegmentASCII[c - 32] & mask) << std::endl;
        if (SixteenSegmentASCII[c - 32] & mask) {
            segmentName = segmentNames[i];
            lightSegment(segmentName);
        } else {
            segmentName = segmentNames[i];
            unlightSegment(segmentName);
        }
        mask = mask >> 1;
    }

}

void SegmentDisplay::closeAllSegments() {

    for (auto segment: segments) {
        unlightSegment(segment.first);
    }

}



