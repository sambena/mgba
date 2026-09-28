/* Copyright (c) 2013-2016 Jeffrey Pfau
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef GB_RENDERER_SOFTWARE_H
#define GB_RENDERER_SOFTWARE_H

#include <mgba-util/common.h>

CXX_GUARD_START

#include <mgba/core/core.h>
#include <mgba/internal/gb/gb.h>
#include <mgba/internal/gb/video.h>

struct GBVideoRendererSprite {
	struct GBObj obj;
	int8_t index;
};

// Golden Coins: what drew every pixel of a frame, for a frontend that draws the frame again.
enum {
	GB_CAPTURE_BG = 0,
	GB_CAPTURE_WINDOW = 1,
	GB_CAPTURE_OBJ = 2,
	GB_CAPTURE_NONE = 3, // nothing drawn: the screen is off, or cleared
};

#define GB_CAPTURE_NO_OBJ 0xFF

struct GBVideoCapture {
	// GB_CAPTURE_*
	uint8_t layer[GB_VIDEO_VERTICAL_PIXELS][GB_VIDEO_HORIZONTAL_PIXELS];
	// The colour's number in its tile (bits 0-1), and its palette (bits 2-5: 0-7 of the
	// background's, 8-15 of the objects'); on the Game Boy, objects use 8 and 9
	uint8_t color[GB_VIDEO_VERTICAL_PIXELS][GB_VIDEO_HORIZONTAL_PIXELS];
	// The tile, as a number of 16 bytes into VRAM: 0-383 in bank 0, 384-767 in bank 1
	uint16_t tile[GB_VIDEO_VERTICAL_PIXELS][GB_VIDEO_HORIZONTAL_PIXELS];
	// The object's number in OAM, or GB_CAPTURE_NO_OBJ
	uint8_t obj[GB_VIDEO_VERTICAL_PIXELS][GB_VIDEO_HORIZONTAL_PIXELS];
	// The scroll and the window as each line began to be drawn
	uint8_t scx[GB_VIDEO_VERTICAL_PIXELS];
	uint8_t scy[GB_VIDEO_VERTICAL_PIXELS];
	uint8_t wx[GB_VIDEO_VERTICAL_PIXELS];
	uint8_t wy[GB_VIDEO_VERTICAL_PIXELS];
	uint8_t lcdc[GB_VIDEO_VERTICAL_PIXELS];
	uint32_t frames; // frames finished since the capture was attached
};

struct GBVideoSoftwareRenderer {
	struct GBVideoRenderer d;

	color_t* outputBuffer;
	int outputBufferStride;

	// TODO: Implement the pixel FIFO
	uint16_t row[GB_VIDEO_HORIZONTAL_PIXELS + 8];

	color_t palette[192];
	uint8_t lookup[192];

	uint32_t* temporaryBuffer;

	uint8_t scy;
	uint8_t scx;
	uint8_t wy;
	uint8_t wx;
	uint8_t currentWy;
	uint8_t currentWx;
	int lastY;
	int lastX;
	bool hasWindow;

	GBRegisterLCDC lcdc;
	enum GBModel model;

	struct GBVideoRendererSprite obj[GB_VIDEO_MAX_LINE_OBJ];
	int objMax;

	int16_t objOffsetX;
	int16_t objOffsetY;
	int16_t offsetScx;
	int16_t offsetScy;
	int16_t offsetWx;
	int16_t offsetWy;

	int sgbTransfer;
	uint8_t sgbPacket[128];
	uint8_t sgbCommandHeader;
	bool sgbBorders;
	uint32_t sgbBorderMask[18];

	uint8_t lastHighlightAmount;

	// Golden Coins: filled as lines are drawn, when not NULL
	struct GBVideoCapture* capture;
	uint16_t rowTile[GB_VIDEO_HORIZONTAL_PIXELS + 8];
	uint8_t rowObj[GB_VIDEO_HORIZONTAL_PIXELS + 8];
	uint8_t rowLayer[GB_VIDEO_HORIZONTAL_PIXELS + 8];
	uint8_t drawingLayer;
};

void GBVideoSoftwareRendererCreate(struct GBVideoSoftwareRenderer*);

CXX_GUARD_END

#endif
