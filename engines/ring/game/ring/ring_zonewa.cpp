/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "ring/game/ring/ring_zonewa.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneWARing::ZoneWARing(ApplicationRing *application) : _app(application) {
}

ZoneWARing::~ZoneWARing() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneWARing::onInit() {
	debugC(1, kRingDebugLogic, "Initializing Brnnnhilde zone (WA)...");

	_app->setZoneAndEnableBag(kZoneWA);

	_app->puzzleAdd(kPuzzle51001);
	_app->puzzleAddBackgroundImage(kPuzzle51001, "TR_WA_A06.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51002);
	_app->puzzleAddBackgroundImage(kPuzzle51002, "TR_WA_A09.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51003);
	_app->puzzleAddBackgroundImage(kPuzzle51003, "TR_WA_AS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51004);
	_app->puzzleAddBackgroundImage(kPuzzle51004, "TR_WA_BS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51005);
	_app->puzzleAddBackgroundImage(kPuzzle51005, "TR_WA_CS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51006);
	_app->puzzleAddBackgroundImage(kPuzzle51006, "TR_WA_DS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51007);
	_app->puzzleAddBackgroundImage(kPuzzle51007, "TR_WA_ES.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51008);
	_app->puzzleAddBackgroundImage(kPuzzle51008, "TR_WA_FS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51009);
	_app->puzzleAddBackgroundImage(kPuzzle51009, "TR_WA_GS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51010);
	_app->puzzleAddBackgroundImage(kPuzzle51010, "TR_WA_HS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51011);
	_app->puzzleAddBackgroundImage(kPuzzle51011, "TR_WA_IS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51012);
	_app->puzzleAddBackgroundImage(kPuzzle51012, "TR_WA_JS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle51013);
	_app->puzzleAddBackgroundImage(kPuzzle51013, "TR_WA_KS.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50100);
	_app->puzzleAddBackgroundImage(kPuzzle50100, "WAS01N08P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50601);
	_app->puzzleAddBackgroundImage(kPuzzle50601, "WAS06N01P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50701);
	_app->puzzleAddBackgroundImage(kPuzzle50701, "WAS07N01P01S01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50702);
	_app->puzzleAddBackgroundImage(kPuzzle50702, "WAS07N01P01S02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50703);
	_app->puzzleAddBackgroundImage(kPuzzle50703, "WAS07N01P01S03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50602);
	_app->puzzleAddBackgroundImage(kPuzzle50602, "WAS06N01P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50001);
	_app->puzzleAddBackgroundImage(kPuzzle50001, "WAS00N01P01L01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50002);
	_app->puzzleAddBackgroundImage(kPuzzle50002, "WAS00N01P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50303);
	_app->puzzleAddBackgroundImage(kPuzzle50303, "WAS03N03P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50304);
	_app->puzzleAddBackgroundImage(kPuzzle50304, "WAS03N04P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50400);
	_app->puzzleAddBackgroundImage(kPuzzle50400, "WAS04N02P01L01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50401);
	_app->puzzleAddBackgroundImage(kPuzzle50401, "WAS04N02P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50501);
	_app->puzzleAddBackgroundImage(kPuzzle50501, "WAS05N02P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50502);
	_app->puzzleAddBackgroundImage(kPuzzle50502, "WAS05N02P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50503);
	_app->puzzleAddBackgroundImage(kPuzzle50503, "WAS05N02P03.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle50504);
	_app->puzzleAddBackgroundImage(kPuzzle50504, "WAS05N02P04.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(50001, "WAS00N01", 0, 0);
	_app->rotationAdd(50101, "WAS01N01", 0, 0);
	_app->rotationAdd(50102, "WAS01N02", 0, 0);
	_app->rotationAdd(50103, "WAS01N03", 0, 2);
	_app->rotationAdd(50104, "WAS01N04", 0, 0);
	_app->rotationAdd(50105, "WAS01N05", 0, 0);
	_app->rotationAdd(50106, "WAS01N06", 0, 0);
	_app->rotationAdd(50107, "WAS01N07", 0, 0);
	_app->rotationAdd(50108, "WAS01N08", 0, 0);
	_app->rotationAdd(50201, "WAS02N01", 0, 3);
	_app->rotationAdd(50202, "WAS02N02", 0, 3);
	_app->rotationAdd(50301, "WAS03N01", 0, 1);
	_app->rotationAdd(50302, "WAS03N02", 0, 1);
	_app->rotationAdd(50303, "WAS03N03", 0, 1);
	_app->rotationAdd(50304, "WAS03N04", 0, 6);
	_app->rotationSetCompressionBufferLength(50304, 1400000);
	_app->rotationAdd(50401, "WAS04N01", 0, 1);
	_app->rotationAdd(50402, "WAS04N02", 0, 2);
	_app->rotationAdd(50501, "WAS05N01", 0, 2);
	_app->rotationAdd(50502, "WAS05N02", 0, 7);
	_app->rotationAdd(50601, "WAS06N01", 0, 0);
	_app->rotationAdd(50602, "WAS06N02", 0, 0);
	_app->rotationAdd(50701, "WAS07N01", 0, 2);
	_app->rotationSetCompressionBufferLength(50701, 1600000);
	_app->rotationAddMovabilityToRotation(50001, 50101, "1883", Common::Rect(3341, -249, 3599, 242), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50001, 0, 312.0f, 0.3f, 85.3f, 0, 0, 354.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50001, 50101, "1883", Common::Rect(0, -249, 145, 242), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50001, 1, 312.0f, 0.3f, 85.3f, 0, 2, 354.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50101, 50102, "1884", Common::Rect(284, -170, 525, 127), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50101, 0, 357.0f, 0.3f, 85.3f, 0, 2, 357.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50101, 50001, "1885", Common::Rect(1747, -202, 1961, 179), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50101, 1, 140.0f, 0.3f, 85.3f, 0, 2, 140.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50102, 50103, "1886", Common::Rect(33, -280, 377, 52), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50102, 0, 329.0f, 0.3f, 85.3f, 0, 2, 254.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50102, 50101, "1887", Common::Rect(2049, -165, 2378, 198), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50102, 1, 171.0f, 0.3f, 85.3f, 0, 2, 142.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50102, 50201, "1888", Common::Rect(1158, -263, 1524, 240), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50102, 2, 90.0f, 0.3f, 85.3f, 0, 0, 90.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50103, 50104, "1889", Common::Rect(2729, -247, 3072, 81), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50103, 0, 237.0f, 0.3f, 85.3f, 0, 2, 164.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50103, 50102, "1890", Common::Rect(1453, -76, 1738, 293), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50103, 1, 122.0f, 0.3f, 85.3f, 0, 2, 174.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50103, 50701, "1891", Common::Rect(2033, -111, 2416, 340), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50103, 2, 180.0f, 0.3f, 85.3f, 0, 0, 180.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50103, 50701, "", Common::Rect(2033, -111, 2416, 340), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50103, 3, 180.0f, 0.3f, 85.3f, 0, 0, 180.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50104, 50105, "1892", Common::Rect(1882, -291, 2163, 109), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50104, 0, 148.0f, 0.3f, 85.3f, 0, 2, 70.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50104, 50103, "1893", Common::Rect(446, -85, 782, 301), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50104, 1, 25.0f, 0.3f, 85.3f, 0, 2, 117.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50104, 50301, "1894", Common::Rect(3005, -310, 3350, 221), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50104, 2, 274.0f, 0.3f, 85.3f, 0, 0, 270.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50105, 50106, "1895", Common::Rect(1000, -242, 1311, 115), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50105, 0, 68.0f, 0.3f, 85.3f, 0, 2, 335.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50105, 50104, "1896", Common::Rect(3248, -80, 3510, 272), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50105, 1, 294.0f, 0.3f, 85.3f, 0, 2, 14.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50105, 50401, "1897", Common::Rect(2056, -268, 2425, 244), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50105, 2, 180.0f, 0.3f, 85.3f, 0, 0, 180.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50106, 50107, "1898", Common::Rect(82, -261, 355, 59), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50106, 0, 330.0f, 0.3f, 85.3f, 0, 2, 250.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50106, 50105, "1899", Common::Rect(2337, -87, 2612, 268), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50106, 1, 206.0f, 0.3f, 85.3f, 0, 2, 295.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50106, 50501, "1900", Common::Rect(1175, -313, 1546, 223), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50106, 2, 90.0f, 0.3f, 85.3f, 0, 0, 90.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50107, 50108, "1901", Common::Rect(2715, -132, 3090, 104), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50107, 0, 250.0f, 0.3f, 85.3f, 0, 2, 250.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50107, 50106, "1902", Common::Rect(1281, -81, 1546, 277), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50107, 1, 100.0f, 0.3f, 85.3f, 0, 2, 200.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50108, 50107, "1904", Common::Rect(987, -216, 1350, 129), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50108, 0, 74.0f, 0.3f, 85.3f, 0, 2, 74.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50201, 50102, "1905", Common::Rect(2768, -516, 3506, 516), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50201, 0, 270.0f, 0.3f, 85.3f, 0, 0, 270.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50201, 50202, "1906", Common::Rect(1146, -118, 1437, 172), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50201, 1, 90.0f, 0.3f, 85.3f, 0, 0, 90.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50202, 50201, "1907", Common::Rect(2898, -209, 3267, 132), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50202, 0, 265.0f, 0.3f, 85.3f, 0, 2, 265.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50301, 50104, "1908", Common::Rect(975, -520, 1699, 520), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50301, 0, 90.0f, 0.3f, 85.3f, 0, 0, 90.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50301, 50302, "1909", Common::Rect(2896, -146, 3529, 231), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50301, 1, 272.0f, 0.3f, 85.3f, 0, 2, 272.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50301, 50104, "1910", Common::Rect(975, -520, 1699, 520), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50301, 2, 90.0f, 0.3f, 85.3f, 0, 0, 90.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50301, 50302, "1911", Common::Rect(2896, -146, 3529, 231), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50301, 3, 272.0f, 0.3f, 85.3f, 0, 2, 272.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50302, 50301, "1912", Common::Rect(1225, -209, 1634, 143), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50302, 0, 97.0f, 0.3f, 85.3f, 0, 2, 97.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50302, 50303, "1913", Common::Rect(548, -237, 762, 195), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50302, 1, 23.0f, 0.3f, 85.3f, 0, 2, 265.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50302, 50304, "1914", Common::Rect(2112, -78, 2288, 137), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50302, 2, 172.0f, 0.3f, 85.3f, 0, 2, 172.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50302, 50301, "1915", Common::Rect(1225, -209, 1634, 143), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50302, 3, 97.0f, 0.3f, 85.3f, 0, 2, 97.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50302, 50303, "1916", Common::Rect(548, -237, 762, 195), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50302, 4, 23.0f, 0.3f, 85.3f, 0, 2, 265.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50302, 50304, "1917", Common::Rect(2112, -78, 2288, 137), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50302, 5, 172.0f, 0.3f, 85.3f, 0, 2, 172.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50303, 50302, "1918", Common::Rect(2221, 216, 2647, 518), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50303, 0, 191.0f, 0.3f, 85.3f, 0, 2, 191.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50303, 50302, "1919", Common::Rect(2221, 216, 2647, 518), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50303, 1, 191.0f, 0.3f, 85.3f, 0, 2, 191.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50304, 50302, "", Common::Rect(217, -87, 506, 301), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50304, 0, 345.0f, 0.3f, 85.3f, 0, 2, 345.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50304, 50302, "", Common::Rect(217, -87, 506, 301), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50304, 0, 345.0f, 0.3f, 85.3f, 0, 2, 345.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50401, 50105, "1920", Common::Rect(123, -510, 866, 510), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50401, 0, 0, 0.3f, 85.3f, 0, 0, 0, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50401, 50402, "1921", Common::Rect(2148, -275, 2360, 106), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50401, 1, 180.0f, 0.3f, 85.3f, 0, 2, 180.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50401, 50402, "1922", Common::Rect(2148, -275, 2360, 106), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50401, 2, 180.0f, 0.3f, 85.3f, 0, 2, 180.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50402, 50401, "1923", Common::Rect(386, 122, 662, 331), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50402, 0, 0, 0.3f, 85.3f, 0, 0, 0, 0.3f, 85.3f);
	_app->rotationAddMovabilityToPuzzle(50402, kPuzzle50400, "1924", Common::Rect(2191, 296, 2445, 481), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(50402, 1, 180.0f, 26.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(50402, kPuzzle50401, "", Common::Rect(2169, 47, 2392, 200), false, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(50402, 2, 180.0f, 26.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToRotation(50501, 50106, "1925", Common::Rect(2837, -480, 3327, 355), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50501, 0, 270.0f, 0.3f, 85.3f, 0, 0, 270.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50501, 50502, "1926", Common::Rect(1100, -202, 1420, 174), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50501, 1, 90.0f, 0.3f, 85.3f, 0, 2, 90.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50502, 50501, "1927", Common::Rect(2916, -277, 3179, 174), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50502, 0, 270.0f, 0.3f, 85.3f, 0, 0, 270.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50601, 50108, "1928", Common::Rect(907, -273, 1406, 247), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50601, 0, 70.0f, 0.3f, 85.3f, 0, 2, 70.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50601, 50602, "1929", Common::Rect(3387, -102, 3599, 348), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50601, 1, 296.0f, 0.3f, 85.3f, 0, 2, 296.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50601, 50602, "1929", Common::Rect(0, -102, 195, 348), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50601, 2, 296.0f, 0.3f, 85.3f, 0, 2, 296.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToPuzzle(50601, kPuzzle50601, "", Common::Rect(2782, 317, 3250, 552), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(50601, 3, 180.0f, 26.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToPuzzle(50601, kPuzzle50602, "", Common::Rect(2643, -80, 3306, 177), true, kCursorHotspot, 0);
	_app->rotationSetMovabilityToPuzzle(50601, 4, 180.0f, 26.0f, 85.3f, 0, 2);
	_app->rotationAddMovabilityToRotation(50602, 50601, "1930", Common::Rect(1408, -181, 1666, 237), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50602, 0, 117.0f, 0.3f, 85.3f, 0, 2, 117.0f, 0.3f, 85.3f);
	_app->rotationAddMovabilityToRotation(50701, 50103, "1931", Common::Rect(225, -136, 701, 378), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(50701, 0, 0, 0.3f, 85.3f, 0, 0, 0, 0.3f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50400, 50402, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50400, 0, 180.0f, 26.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50401, 50402, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50401, 0, 180.0f, 26.0f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50303, 50303, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50303, 0, 265.0f, 0.3f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50304, 50304, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50304, 0, 130.0f, 0.3f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50601, 50601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50601, 0, 250.0f, 26.0f, 85.3f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle50601, kPuzzle50602, "", Common::Rect(0, 0, 640, 32), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle50602, 50601, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50602, 0, 250.0f, 0.3f, 85.3f);
	_app->puzzleAddMovabilityToPuzzle(kPuzzle50602, kPuzzle50601, "", Common::Rect(0, 0, 640, 32), true, kCursorBack, 0);
	_app->puzzleAddMovabilityToRotation(kPuzzle50501, 50502, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50501, 0, 90.0f, 0.3f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50502, 50502, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50502, 0, 90.0f, 0.3f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50503, 50502, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50503, 0, 90.0f, 0.3f, 85.3f);
	_app->puzzleAddMovabilityToRotation(kPuzzle50504, 50502, "", Common::Rect(0, 420, 640, 464), true, kCursorBack, 0);
	_app->puzzleSetMovabilityToRotation(kPuzzle50504, 0, 90.0f, 0.3f, 85.3f);
	_app->objectAdd(kObject51000, "", "", 1);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 0, kPuzzle51001, "TR_WA_A06", kImageTypeBMP, Common::Point(262, 151), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 1, kPuzzle51002, "TR_WA_A09", kImageTypeBMP, Common::Point(139, 94), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 2, kPuzzle51004, "TR_WA_BS", kImageTypeBMP, Common::Point(264, 189), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 3, kPuzzle51007, "TR_WA_ES", kImageTypeBMP, Common::Point(218, 18), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 4, kPuzzle51009, "TR_WA_GS", kImageTypeBMP, Common::Point(228, 82), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 5, kPuzzle51011, "TR_WA_IS", kImageTypeBMP, Common::Point(323, 184), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 6, kPuzzle51012, "TR_WA_JS", kImageTypeBMP, Common::Point(223, 16), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAddPresentation(kObject51000);
	_app->objectPresentationAddAnimationToPuzzle(kObject51000, 7, kPuzzle51013, "TR_WA_KS", kImageTypeBMP, Common::Point(210, 70), kDrawType1, 1000, 19, 12.5f, 4);
	_app->objectAdd(kObjectMagicLance, "Beam of light", "WA_Lance", 1);
	_app->objectAddBagAnimation(kObjectMagicLance, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMagicLance, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMagicLance, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectFeather, "Feather", "WA_Feather", 1);
	_app->objectAddBagAnimation(kObjectFeather, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFeather, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFeather, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectDeadLeaf, "DeadLeaf", "WA_ItemLeaf", 1);
	_app->objectAddBagAnimation(kObjectDeadLeaf, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectDeadLeaf, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectDeadLeaf, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectInk, "Ink", "WA_Ink", 8);
	_app->objectAddBagAnimation(kObjectInk, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectInk, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectInk, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectInk, kPuzzle50100, Common::Rect(446, 399, 488, 457), true, kCursorHotspot, 0);
	_app->objectAdd(kObjectPaper, "Paper", "WA_Paper", 8);
	_app->objectAddBagAnimation(kObjectPaper, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectPaper, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectPaper, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectPaper, kPuzzle50100, Common::Rect(487, 386, 526, 453), true, kCursorHotspot, 1);
	_app->objectAdd(kObjectStylet, "Stylet", "WA_Staylet", 8);
	_app->objectAddBagAnimation(kObjectStylet, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectStylet, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectStylet, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectStylet, kPuzzle50100, Common::Rect(526, 414, 556, 450), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObjectStylet, kPuzzle50100, Common::Rect(220, 395, 283, 456), true, kCursorHotspot, 8);
	_app->objectAdd(kObjectInkedStylet, "Ink & Stylet", "WA_StyletInk", 8);
	_app->objectAddBagAnimation(kObjectInkedStylet, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectInkedStylet, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectInkedStylet, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectInkedStylet, kPuzzle50100, Common::Rect(326, 399, 374, 446), true, kCursorHotspot, 9);
	_app->objectAdd(kObject50105, "Ashes", "", 1);
	_app->objectSetActiveCursor(kObject50105, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObject50105, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObject50105, kPuzzle50100, Common::Rect(318, 31, 379, 102), true, kCursorHotspot, 0);
	_app->objectAdd(kObject50100, "", "", 1);
	_app->objectAddRotationAccessibility(kObject50100, 50108, Common::Rect(2701, -231, 3260, 345), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 0, kPuzzle50100, "WAS01N08P01L01_A.bmp", Common::Point(450, 401), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 1, kPuzzle50100, "WAS01N08P01L01_B.bmp", Common::Point(490, 388), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 2, kPuzzle50100, "WAS01N08P01L01_C.bmp", Common::Point(526, 414), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 3, kPuzzle50100, "WAS01N08P01L03.bmp", Common::Point(221, 397), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 4, kPuzzle50100, "WAS01N08P01L02.bmp", Common::Point(326, 401), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 5, kPuzzle50100, "WAS01N08P01L05.bmp", Common::Point(218, 398), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 6, kPuzzle50100, "WAS01N08P01L04.bmp", Common::Point(325, 400), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddAnimationToPuzzle(kObject50100, 7, kPuzzle50100, "WAS01N08P01S01", kImageTypeBMP, Common::Point(270, 101), kDrawType1, 1000, 50, 12.5f, 6);
	_app->objectPresentationSetAnimationStartFrame(kObject50100, 7, 1);
	_app->objectPresentationSetAnimationOnPuzzle(kObject50100, 7, 0, 50003);
	_app->objectAddPresentation(kObject50100);
	_app->objectPresentationAddAnimationToPuzzle(kObject50100, 8, kPuzzle50100, "WAS01N08P01S02", kImageTypeBMP, Common::Point(316, 30), kDrawType1, 1000, 50, 12.5f, 6);
	_app->objectPresentationSetAnimationOnPuzzle(kObject50100, 8, 0, 50004);
	_app->objectPresentationAddImageToPuzzle(kObject50100, 8, kPuzzle50100, "WAS01N08P01S01.bmp", Common::Point(270, 101), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObject50100, 0);
	_app->objectPresentationShow(kObject50100, 1);
	_app->objectPresentationShow(kObject50100, 2);
	_app->objectAdd(kObject50600, "", "", 1);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddAnimationToPuzzle(kObject50600, 0, kPuzzle50601, "WAS06N01P01S01", kImageTypeBMP, Common::Point(171, 182), kDrawType1, 1000, 50, 12.5f, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObject50600, 0, 0, 50001);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddAnimationToPuzzle(kObject50600, 1, kPuzzle50602, "WAS06N01P02S01", kImageTypeBMP, Common::Point(77, 134), kDrawType1, 1000, 30, 12.5f, 4);
	_app->objectPresentationSetAnimationOnPuzzle(kObject50600, 1, 0, 50002);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddImageToPuzzle(kObject50600, 2, kPuzzle50601, "WAS06N01P01L01.bmp", Common::Point(206, 149), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddImageToPuzzle(kObject50600, 3, kPuzzle50601, "WAS06N01P01L02.bmp", Common::Point(383, 146), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddImageToPuzzle(kObject50600, 4, kPuzzle50601, "WAS06N01P01L03.bmp", Common::Point(225, 282), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddImageToPuzzle(kObject50600, 5, kPuzzle50601, "WAS06N01P01L04.bmp", Common::Point(380, 294), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddImageToPuzzle(kObject50600, 6, kPuzzle50602, "WAS06N01P02L01.bmp", Common::Point(115, 178), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObject50600);
	_app->objectPresentationAddImageToPuzzle(kObject50600, 7, kPuzzle50602, "WAS06N01P02L02.bmp", Common::Point(114, 158), true, kDrawType1, 1000);
	_app->objectPresentationShow(kObject50600, 0);
	_app->objectPresentationShow(kObject50600, 1);
	_app->objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(208, 148, 237, 177), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(384, 148, 411, 172), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(227, 281, 253, 308), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject50600, kPuzzle50601, Common::Rect(381, 290, 410, 322), true, kCursorHotspot, 3);
	_app->objectAdd(kObject50001, "", "", 1);
	_app->objectAddPresentation(kObject50001);
	_app->objectPresentationAddAnimationToPuzzle(kObject50001, 0, kPuzzle50002, "WAS00N01P01S01", kImageTypeBMP, Common::Point(414, 63), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject50001);
	_app->objectPresentationAddAnimationToPuzzle(kObject50001, 1, kPuzzle50001, "WAS00N01P01S02", kImageTypeBMP, Common::Point(256, 263), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectGolem1, "Golem", "FO_Golem", 1);
	_app->objectAddBagAnimation(kObjectGolem1, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectGolem1, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectGolem1, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectGolem1, kPuzzle50400, Common::Rect(232, 168, 422, 403), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectGolem1);
	_app->objectPresentationAddImageToRotation(kObjectGolem1, 0, 50402, 0);
	_app->objectPresentationAddImageToRotation(kObjectGolem1, 0, 50401, 0);
	_app->objectAddPresentation(kObjectGolem1);
	_app->objectPresentationAddImageToPuzzle(kObjectGolem1, 1, kPuzzle50400, "WAS04N02P01L02.bmp", Common::Point(212, 178), true, kDrawType1, 998);
	_app->objectAdd(kObjectFronthead, "Head front", "WA_GolemHeadF", 8);
	_app->objectAddBagAnimation(kObjectFronthead, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFronthead, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFronthead, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectFronthead, kPuzzle50400, Common::Rect(298, 159, 355, 233), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectFronthead, kPuzzle50400, Common::Rect(215,  92, 269, 160), false, kCursorHotspot, 1);
	_app->objectAddPresentation(kObjectFronthead);
	_app->objectPresentationAddImageToPuzzle(kObjectFronthead, 0, kPuzzle50400, "WAS04N02P01L09.bmp", Common::Point(296, 160), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectFronthead);
	_app->objectPresentationAddImageToPuzzle(kObjectFronthead, 1, kPuzzle50400, "WAS04N02P01L03.bmp", Common::Point(209, 87), true, kDrawType1, 1000);
	_app->objectAdd(kObjectBackhead, "Head back", "WA_GolemHeadB", 8);
	_app->objectAddBagAnimation(kObjectBackhead, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBackhead, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBackhead, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectBackhead, kPuzzle50400, Common::Rect(298, 159, 355, 233), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectBackhead, kPuzzle50400, Common::Rect(399,  93, 450, 158), false, kCursorHotspot, 20);
	_app->objectAddPresentation(kObjectBackhead);
	_app->objectPresentationAddImageToPuzzle(kObjectBackhead, 0, kPuzzle50400, "WAS04N02P01L21.bmp", Common::Point(301, 157), true, kDrawType1, 999);
	_app->objectAddPresentation(kObjectBackhead);
	_app->objectPresentationAddImageToPuzzle(kObjectBackhead, 1, kPuzzle50400, "WAS04N02P01L06.bmp", Common::Point(403, 89), true, kDrawType1, 1000);
	_app->objectAdd(kObjectBelly, "Belly", "WA_GolemStomak", 8);
	_app->objectAddBagAnimation(kObjectBelly, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBelly, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBelly, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectBelly, kPuzzle50400, Common::Rect(298, 257, 358, 299), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectBelly, kPuzzle50400, Common::Rect(135, 301, 212, 358), false, kCursorHotspot, 300);
	_app->objectAddPresentation(kObjectBelly);
	_app->objectPresentationAddImageToPuzzle(kObjectBelly, 0, kPuzzle50400, "WAS04N02P01L13.bmp", Common::Point(292, 252), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectBelly, 0, kPuzzle50400, "WAS04N02P01L14.bmp", Common::Point(302, 293), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectBelly);
	_app->objectPresentationAddImageToPuzzle(kObjectBelly, 1, kPuzzle50400, "WAS04N02P01L07.bmp", Common::Point(142, 302), true, kDrawType1, 1000);
	_app->objectAdd(kObjectRightArm, "Arm right", "WA_GolemArmR", 8);
	_app->objectAddBagAnimation(kObjectRightArm, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectRightArm, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectRightArm, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectRightArm, kPuzzle50400, Common::Rect(241, 213, 276, 253), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectRightArm, kPuzzle50400, Common::Rect(151, 206, 200, 255), false, kCursorHotspot, 4000);
	_app->objectAddPresentation(kObjectRightArm);
	_app->objectPresentationAddImageToPuzzle(kObjectRightArm, 0, kPuzzle50400, "WAS04N02P01L10.bmp", Common::Point(224, 203), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectRightArm);
	_app->objectPresentationAddImageToPuzzle(kObjectRightArm, 1, kPuzzle50400, "WAS04N02P01L04.bmp", Common::Point(147, 206), true, kDrawType1, 1000);
	_app->objectAdd(kObjectLeftArm, "Arm left", "WA_GolemArmL", 8);
	_app->objectAddBagAnimation(kObjectLeftArm, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectLeftArm, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectLeftArm, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectLeftArm, kPuzzle50400, Common::Rect(380, 217, 414, 249), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectLeftArm, kPuzzle50400, Common::Rect(443, 211, 488, 253), false, kCursorHotspot, 50000);
	_app->objectAddPresentation(kObjectLeftArm);
	_app->objectPresentationAddImageToPuzzle(kObjectLeftArm, 0, kPuzzle50400, "WAS04N02P01L11.bmp", Common::Point(375, 210), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectLeftArm);
	_app->objectPresentationAddImageToPuzzle(kObjectLeftArm, 1, kPuzzle50400, "WAS04N02P01L05.bmp", Common::Point(440, 208), true, kDrawType1, 1000);
	_app->objectAdd(kObjectLegs, "Legs", "WA_GolemLeg", 8);
	_app->objectAddBagAnimation(kObjectLegs, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectLegs, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectLegs, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectLegs, kPuzzle50400, Common::Rect(268, 306, 384, 391), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectLegs, kPuzzle50400, Common::Rect(445, 297, 534, 349), false, kCursorHotspot, 600000);
	_app->objectAddPresentation(kObjectLegs);
	_app->objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L15.bmp", Common::Point(269, 282), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L16.bmp", Common::Point(268, 299), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L17.bmp", Common::Point(263, 307), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L18.bmp", Common::Point(365, 285), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L19.bmp", Common::Point(348, 301), true, kDrawType1, 1000);
	_app->objectPresentationAddImageToPuzzle(kObjectLegs, 0, kPuzzle50400, "WAS04N02P01L20.bmp", Common::Point(334, 309), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectLegs);
	_app->objectPresentationAddImageToPuzzle(kObjectLegs, 1, kPuzzle50400, "WAS04N02P01L08.bmp", Common::Point(440, 301), true, kDrawType1, 1000);
	_app->objectAdd(kObjectHeart, "Heart", "WA_GolemHeart", 8);
	_app->objectAddBagAnimation(kObjectHeart, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectHeart, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectHeart, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectHeart, kPuzzle50400, Common::Rect(342, 231, 363, 251), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectHeart, kPuzzle50400, Common::Rect(325,  50, 340,  95), false, kCursorHotspot, 7000000);
	_app->objectAddPresentation(kObjectHeart);
	_app->objectPresentationAddImageToPuzzle(kObjectHeart, 0, kPuzzle50400, "WAS04N02P01L12.bmp", Common::Point(343, 231), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectHeart);
	_app->objectPresentationAddImageToPuzzle(kObjectHeart, 1, kPuzzle50400, "WAS04N02P01L22.bmp", Common::Point(315, 74), true, kDrawType1, 1000);
	_app->objectAdd(kObject50451, "", "WA_glava", 8);
	_app->objectAdd(kObject50452, "", "WA_droka", 8);
	_app->objectAdd(kObject50453, "", "WA_lroka", 8);
	_app->objectAdd(kObject50454, "", "WA_trup", 8);
	_app->objectAdd(kObject50455, "", "WA_lnoga", 8);
	_app->objectAdd(kObject50456, "", "WA_dnoga", 8);
	_app->objectAdd(kObject50457, "", "WA_srce", 8);

	int16 val = 115;
	for (uint32 i = 0; i < 7; i++) {
		_app->objectSetActiveCursor((ObjectId)(kObject50451 + i), Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
		_app->objectSetPassiveCursor((ObjectId)(kObject50451 + i), Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
		_app->objectAddPuzzleAccessibility((ObjectId)(kObject50451 + i), kPuzzle50401, Common::Rect(125, val, 160, i >= 6 ? (int16)(val + 35) : val), i >= 6 ? false : true, kCursorHotspot, 0);
		_app->objectAddPresentation((ObjectId)(kObject50451 + i));
		_app->objectPresentationAddImageToPuzzle((ObjectId)(kObject50451 + i), 0, kPuzzle50401, Common::String::format("WAS04N02P02_L%d.tga", i + 1), Common::Point(125, val), true, kDrawType3, 1000);
		_app->objectPresentationShow((ObjectId)(kObject50451 + i));
		val += 37;
	}

	for (uint32 i = 0; i < 7; i++) {
		uint32 l = 1;
		for (uint32 j = 115; j < (115 + 7 * 37); j += 37) {
			for (uint32 k = 162; k < (162 + 7 * 53); k += 53) {
				_app->objectAddPuzzleAccessibility((ObjectId)(kObject50451 + i), kPuzzle50401, Common::Rect((int16)k, (int16)j, (int16)(k + 53), (int16)(j + 37)), false, kCursorHotspot, l * 10);
			}
			l++;
		}
	}

	_app->objectAdd(kObjectMagnet, "", "", 8);

	uint32 l = 1;
	for (uint32 j = 115; j < (115 + 7 * 37); j += 37) {
		for (uint32 k = 162; k < (162 + 7 * 53); k += 53) {
			_app->objectAddPuzzleAccessibility(kObjectMagnet, kPuzzle50401, Common::Rect((int16)k, (int16)j, (int16)(k + 53), (int16)(j + 37)), false, kCursorHotspot, l * 10);
		}
		l++;
	}

	_app->objectAdd(kObjectThread, "Rope", "WA_Rope", 1);
	_app->objectAddBagAnimation(kObjectThread, kImageTypeTGA, kDrawType3, 1, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectThread, Common::Point(22, 22), 1, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectThread, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObjectTree, "", "", 1);
	_app->objectAddPresentation(kObjectTree);
	_app->objectPresentationAddImageToRotation(kObjectTree, 0, 50502, 0);
	_app->objectPresentationAddImageToRotation(kObjectTree, 0, 50501, 0);
	_app->objectAddPresentation(kObjectTree);
	_app->objectPresentationAddImageToRotation(kObjectTree, 1, 50502, 1);
	_app->objectPresentationAddImageToPuzzle(kObjectTree, 1, kPuzzle50501, "WAS05N02P01L01.BMP", Common::Point(281, 164), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectTree);
	_app->objectPresentationAddImageToRotation(kObjectTree, 2, 50502, 2);
	_app->objectPresentationAddImageToPuzzle(kObjectTree, 2, kPuzzle50502, "WAS05N02P02L01.BMP", Common::Point(226, 189), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectTree);
	_app->objectPresentationAddImageToRotation(kObjectTree, 3, 50502, 3);
	_app->objectPresentationAddImageToPuzzle(kObjectTree, 3, kPuzzle50503, "WAS05N02P03L01.BMP", Common::Point(215, 200), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectTree);
	_app->objectPresentationAddImageToRotation(kObjectTree, 4, 50502, 4);
	_app->objectPresentationAddImageToPuzzle(kObjectTree, 4, kPuzzle50504, "WAS05N02P04L01.BMP", Common::Point(213, 140), true, kDrawType1, 1000);
	_app->objectAddPresentation(kObjectTree);
	_app->objectPresentationAddImageToRotation(kObjectTree, 5, 50502, 5);
	_app->objectPresentationShow(kObjectTree, 5);
	_app->objectAddPresentation(kObjectTree);
	_app->objectPresentationAddImageToRotation(kObjectTree, 6, 50502, 6);
	_app->objectPresentationAddImageToRotation(kObjectTree, 6, 50501, 1);
	_app->objectAdd(kObject50700, "", "", 1);
	_app->objectAddRotationAccessibility(kObject50700, 50701, Common::Rect(1975, -568, 2378, -240), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject50700, 50701, Common::Rect(2033, -172, 2411, 289), false, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject50700, 50701, Common::Rect(2079, 211, 2429, 537), false, kCursorHotspot, 2);
	_app->objectAddPresentation(kObject50700);
	_app->objectPresentationAddImageToRotation(kObject50700, 0, 50103, 0);
	_app->objectPresentationAddImageToRotation(kObject50700, 0, 50701, 0);
	_app->objectAddPresentation(kObject50700);
	_app->objectPresentationAddImageToRotation(kObject50700, 1, 50103, 1);
	_app->objectPresentationAddImageToRotation(kObject50700, 1, 50701, 1);
	_app->objectAddPresentation(kObject50700);
	_app->objectPresentationAddAnimationToPuzzle(kObject50700, 2, kPuzzle50701, "WAS07N01P01S01", kImageTypeBMP, Common::Point(293, 187), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject50700);
	_app->objectPresentationAddAnimationToPuzzle(kObject50700, 3, kPuzzle50702, "WAS07N01P01S02", kImageTypeBMP, Common::Point(304, 65), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectCounch1, "Conch", "WA_Conch", 1);
	_app->objectAddBagAnimation(kObjectCounch1, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectCounch1, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectCounch1, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectCounch1, 50304, Common::Rect(1582, -54, 2009, 247), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObjectCounch1, 50304, Common::Rect(2424, -10, 2612, 136), false, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObjectCounch1, kPuzzle50304, Common::Rect(184, 191, 389, 391), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectCounch1);
	_app->objectPresentationAddImageToRotation(kObjectCounch1, 0, 50304, 0);
	_app->objectAddPresentation(kObjectCounch1);
	_app->objectPresentationAddImageToRotation(kObjectCounch1, 1, 50304, 1);
	_app->objectAddPresentation(kObjectCounch1);
	_app->objectPresentationAddImageToRotation(kObjectCounch1, 2, 50304, 2);
	_app->objectAddPresentation(kObjectCounch1);
	_app->objectPresentationAddImageToRotation(kObjectCounch1, 3, 50304, 3);
	_app->objectAddPresentation(kObjectCounch1);
	_app->objectPresentationAddImageToRotation(kObjectCounch1, 4, 50304, 4);
	_app->objectAddPresentation(kObjectCounch1);
	_app->objectPresentationAddImageToRotation(kObjectCounch1, 5, 50304, 5);
	_app->objectAdd(kObjectPhoenix, "", "", 1);
	_app->objectAddRotationAccessibility(kObjectPhoenix, 50303, Common::Rect(2896, -146, 3529, 231), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectPhoenix, kPuzzle50303, Common::Rect(178, 89, 443, 384), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectPhoenix);
	_app->objectPresentationAddImageToRotation(kObjectPhoenix, 0, 50301, 0);
	_app->objectPresentationAddImageToRotation(kObjectPhoenix, 0, 50302, 0);
	_app->objectPresentationAddImageToRotation(kObjectPhoenix, 0, 50303, 0);
	_app->objectAdd(kObjectRopes, "", "", 1);
	_app->objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(653, -270, 775, 125), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(1001, -298, 1023, 130), true, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(1536, -287, 1633, 120), true, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObjectRopes, 50202, Common::Rect(1745, -387, 1965, 211), true, kCursorHotspot, 3);
	_app->objectAddPresentation(kObjectRopes);
	_app->objectPresentationAddImageToRotation(kObjectRopes, 0, 50201, 0);
	_app->objectPresentationAddImageToRotation(kObjectRopes, 0, 50202, 0);
	_app->objectAddPresentation(kObjectRopes);
	_app->objectPresentationAddImageToRotation(kObjectRopes, 1, 50201, 1);
	_app->objectPresentationAddImageToRotation(kObjectRopes, 1, 50202, 1);
	_app->objectAddPresentation(kObjectRopes);
	_app->objectPresentationAddImageToRotation(kObjectRopes, 2, 50201, 2);
	_app->objectPresentationAddImageToRotation(kObjectRopes, 2, 50202, 2);
	_app->objectAdd(kObjectCloud, "", "", 1);
	_app->objectAddRotationAccessibility(kObjectCloud, 50202, Common::Rect(0, -600, 1689, -219), true, kCursorHotspot, 0);
	_app->objectAdd(kObjectCounch, "", "", 1);
	_app->objectAdd(kObjectBark, "Bark", "WA_ItemBark", 9);
	_app->objectAddBagAnimation(kObjectBark, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectBark, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectBark, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectBark, 50302, Common::Rect(2858, 251, 3011, 512), false, kCursorHotspot, 0);
	_app->objectAdd(kObjectFlower, "Flower", "WA_ItemFlower", 9);
	_app->objectAddBagAnimation(kObjectFlower, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectFlower, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectFlower, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectFlower, 50402, Common::Rect(2689, 190, 2937, 301), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectFlower);
	_app->objectPresentationAddImageToRotation(kObjectFlower, 0, 50402, 1);
	_app->objectPresentationShow(kObjectFlower);
	_app->objectAdd(kObjectDragonSword, "Sword", "WA_Sword", 1);
	_app->objectAddBagAnimation(kObjectDragonSword, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectDragonSword, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectDragonSword, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectDragonSword, 50502, Common::Rect(1093, 176, 1383, 381), true, kCursorHotspot, 0);
	_app->objectAdd(kObjectApple, "Apple", "WA_ItemFruit", 1);
	_app->objectAddBagAnimation(kObjectApple, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectApple, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectApple, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectApple, 50502, Common::Rect(407, 190, 513, 289), true, kCursorHotspot, 0);
	_app->objectAdd(kObjectTotems, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50501, Common::Rect(223, 134, 430, 409), true, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50502, Common::Rect(195, 120, 388, 392), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50503, Common::Rect(217,  92, 423, 405), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObjectTotems, kPuzzle50504, Common::Rect(196, 115, 413, 441), true, kCursorHotspot, 3);
	_app->objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect( 947, -20, 1014, 43), true, kCursorHotspot, 10);
	_app->objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect(1091, -29, 1154, 48), true, kCursorHotspot, 11);
	_app->objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect(1469, -31, 1527, 45), true, kCursorHotspot, 12);
	_app->objectAddRotationAccessibility(kObjectTotems, 50502, Common::Rect(1597, -24, 1650, 50), true, kCursorHotspot, 13);
	_app->objectAdd(kObject50601, "", "", 1);
	_app->objectAddRotationAccessibility(kObject50601, 50602, Common::Rect(3225, 17, 3364, 144), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject50601, 50602, Common::Rect(423, -205, 667, 8), true, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject50601, 50602, Common::Rect(868, 0, 996, 95), true, kCursorHotspot, 2);
	_app->soundAdd(50017, kSoundTypeAmbientMusic, "1932.wav", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->puzzleAddAmbientSound(kPuzzle50602, 50017, 100, 0, 1, 1, 10);
	_app->soundSetVolume(50017, 90);
	_app->soundAdd(51002, kSoundTypeAmbientMusic, "1938.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundSetVolume(51002, 95);
	_app->soundAdd(51004, kSoundTypeAmbientMusic, "1939.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundSetVolume(51004, 100);
	_app->soundAdd(51003, kSoundTypeAmbientMusic, "1940.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundSetVolume(51003, 95);
	_app->soundAdd(51001, kSoundTypeAmbientMusic, "1941.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundSetVolume(51001, 95);
	_app->soundAdd(51006, kSoundTypeAmbientMusic, "1942.wav", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundSetVolume(51006, 95);
	_app->soundAdd(51007, kSoundTypeAmbientMusic, "1943.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundSetVolume(51007, 90);
	_app->soundAdd(51008, kSoundTypeAmbientMusic, "1944.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundSetVolume(51008, 95);
	_app->soundAdd(51010, kSoundTypeAmbientMusic, "1945.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(51011, kSoundTypeAmbientMusic, "1946.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(51012, kSoundTypeAmbientMusic, "1947.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(51013, kSoundTypeAmbientMusic, "1948.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().ambientMusic.soundChunck);
	_app->soundAdd(50018, kSoundTypeAmbientEffect, "1949.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().ambientEffect.soundChunck);
	_app->soundAdd(50023, kSoundTypeDialog, "1933.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50024, kSoundTypeDialog, "1934.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50025, kSoundTypeDialog, "1935.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50001, kSoundTypeDialog, "1950.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50002, kSoundTypeDialog, "1951.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50003, kSoundTypeDialog, "1952.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50009, kSoundTypeDialog, "1953.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50010, kSoundTypeDialog, "1954.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50011, kSoundTypeDialog, "1955.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50012, kSoundTypeDialog, "1956.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50013, kSoundTypeDialog, "1957.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50014, kSoundTypeDialog, "1958.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50015, kSoundTypeDialog, "1959.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50016, kSoundTypeDialog, "1960.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50021, kSoundTypeDialog, "1961.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50022, kSoundTypeDialog, "1962.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50026, kSoundTypeDialog, "1963.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50027, kSoundTypeDialog, "1964.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50028, kSoundTypeDialog, "1965.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50029, kSoundTypeDialog, "1966.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50030, kSoundTypeDialog, "1967.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50031, kSoundTypeDialog, "1968.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50032, kSoundTypeDialog, "1969.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50033, kSoundTypeDialog, "1970.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50034, kSoundTypeDialog, "1971.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50035, kSoundTypeDialog, "1972.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(50036, kSoundTypeDialog, "1973.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineByte(50001, 0);
	_app->varDefineByte(50003, 0);
	_app->varDefineWord(50000, 0);
	_app->varDefineByte(50004, 0);
	_app->varDefineByte(50005, 0);
	_app->varDefineDword(50000, 0);
	_app->varDefineWord(50101, 0);
	_app->varDefineWord(50102, 0);
	_app->varDefineWord(50103, 0);
	_app->varDefineWord(50104, 0);
	_app->varDefineWord(50105, 0);
	_app->varDefineByte(50006, 0);
	_app->varDefineByte(50007, 0);
	_app->varDefineByte(50008, 0);
	_app->varDefineByte(50009, 0);
	_app->varDefineByte(50010, 0);
	_app->varDefineByte(50011, 0);
	_app->varDefineByte(50012, 0);

	for (uint32 i = 0; i < 78; i++)
		_app->varDefineDword(51000 + i, 0);
}

void ZoneWARing::onSetup(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectMagicLance);
		_app->bagAdd(kObjectGolem1);
		_app->playMovie("1880");
		_app->playMovie("1881");
		_app->rotationSetAlp(50001, 320.0f);
		_app->rotationSetActive(50001);
		_app->puzzleSetActive(kPuzzle50002);
		_app->soundIsPlaying(50001);
		break;

	case kSetupType10:
		_app->onSetupLoadTimers("bru", 90020, 90024, 90028);
		break;

	case kSetupType999:
		_app->bagAdd(kObjectMagicLance);
		_app->bagAdd(kObjectThread);
		_app->bagAdd(kObjectGolem1);
		_app->bagAdd(kObjectDragonSword);
		_app->bagAdd(kObjectFlower);
		_app->bagAdd(kObjectApple);
		_app->bagAdd(kObjectDeadLeaf);
		_app->bagAdd(kObjectBark);
		_app->varSetFloat(90008, 90.0f);
		_app->objectPresentationShow(kObject1);
		_app->varSetByte(50012, 4);
		_app->puzzleSetActive(kPuzzle50100);
		break;
	}
}

void ZoneWARing::onSwitch(uint32 type) {
	if (type == 0) {
		if (_app->varGetByte(90012))
			_app->setupZone((ZoneId)_app->varGetDword(90016), kSetupType10);
		else
			_app->setupZone(kZoneWA, kSetupTypeNone);
	}
}

void ZoneWARing::onButtonUp(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectFlower:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectFlower);
		_app->objectPresentationHide(kObjectFlower);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		_app->objectSetAccessibilityOff(kObjectFlower);
		break;

	case kObjectDragonSword:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectDragonSword);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		_app->objectPresentationShow(kObjectTree, 0);
		_app->objectSetAccessibilityOff(kObjectDragonSword);
		break;

	case kObjectApple:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectApple);
		_app->objectPresentationHide(kObjectTree, 5);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
		_app->objectSetAccessibilityOff(kObjectApple);
		break;

	case kObject50600:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50007)) {
				_app->objectPresentationHide(kObject50600, 2);
				_app->varSetByte(50007, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 2);
				_app->varSetByte(50007, 1);
			}
			break;

		case 1:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50008)) {
				_app->objectPresentationHide(kObject50600, 3);
				_app->varSetByte(50008, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 3);
				_app->varSetByte(50008, 1);
			}
			break;

		case 2:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50009)) {
				_app->objectPresentationHide(kObject50600, 4);
				_app->varSetByte(50009, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 4);
				_app->varSetByte(50009, 1);
			}
			break;

		case 3:
			_app->soundPlay(50018);
			_app->objectPresentationUnpauseAnimation(kObject50600, 0);
			_app->objectPresentationHide(kObject50600, 6);
			_app->objectPresentationHide(kObject50600, 7);
			_app->objectPresentationShow(kObject50600, 1);
			if (_app->varGetByte(50010)) {
				_app->objectPresentationHide(kObject50600, 5);
				_app->varSetByte(50010, 0);
			} else {
				_app->objectPresentationShow(kObject50600, 5);
				_app->varSetByte(50010, 1);
			}
			break;
		}
		break;

	case kObject50601:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectThread) {
				if (_app->varGetByte(50012) == 6) {
					_app->bagRemove(kObjectThread);
					_app->varSetFloat(90008, 100.0);

					_app->playMovie("1860");
					_app->playMovie("1861");

					switch (_app->getCurrentLanguage()) {
					default:
						_app->playMovieChannel("1862", _app->getLanguageChannel());
						break;

					case kLanguageItalian:
					case kLanguageSpanish:
					case kLanguageDutch:
						_app->playMovieChannel("1863", _app->getLanguageChannel());
						break;

					case kLanguageSwedish:
						_app->playMovieChannel("1864", _app->getLanguageChannel());
						break;
					}

					_app->puzzleSetActive(kPuzzle51001);
					_app->soundPlay(50021);
				}
			}

			_app->cursorDelete();
		}
		break;

	case kObject50700:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle50701);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
		_app->soundPlay(50009);
		break;

	case kObjectTotems:
		if (!_app->bagHasClickedObject()) {
			switch (target) {
			default:
				if (target > 9)
					_app->puzzleSetActive((PuzzleId)(target + 50491));
				break;

			case 0:
				if (_app->varGetWord(50000) % 10 == 1) {
					_app->bagAdd(kObjectFlower);
					_app->objectPresentationHide(kObjectTree, 1);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 1:
				if (_app->varGetWord(50000) % 100 > 1) {
					_app->bagAdd(kObjectApple);
					_app->objectPresentationHide(kObjectTree, 2);
					_app->varSetWord(50000, _app->varGetWord(50000) - 10);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 2:
				if (_app->varGetWord(50000) % 1000 > 11) {
					_app->bagAdd(kObjectDeadLeaf);
					_app->objectPresentationHide(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) - 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 3:
				if (_app->varGetWord(50000) % 10000 > 111) {
					_app->bagAdd(kObjectBark);
					_app->objectPresentationHide(kObjectTree, 4);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1000);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;
			}

			break;
		}

		if (!_app->bagHasClickedObject()) {
			switch (target) {
			default:
				if (target > 9)
					_app->puzzleSetActive((PuzzleId)(target + 50491));
				break;

			case 0:
				if (_app->varGetWord(50000) % 10 == 1) {
					_app->bagAdd(kObjectFlower);
					_app->objectPresentationHide(kObjectTree, 1);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 1:
				if (_app->varGetWord(50000) % 100 > 1) {
					_app->bagAdd(kObjectApple);
					_app->objectPresentationHide(kObjectTree, 2);
					_app->varSetWord(50000, _app->varGetWord(50000) - 10);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 2:
				if (_app->varGetWord(50000) % 1000 > 11) {
					_app->bagAdd(kObjectDeadLeaf);
					_app->objectPresentationHide(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) - 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;

			case 3:
				if (_app->varGetWord(50000) % 10000 > 111) {
					_app->bagAdd(kObjectBark);
					_app->objectPresentationHide(kObjectTree, 4);
					_app->varSetWord(50000, _app->varGetWord(50000) - 1000);
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 2.0f);
				}
				break;
			}

			break;
		}

		if (_app->bagGetClickedObject() != kObjectFlower || target)	{
			if (_app->bagGetClickedObject() != kObjectApple || target != 1) {
				if (_app->bagGetClickedObject() != kObjectDeadLeaf || target != 2) {
					if (_app->bagGetClickedObject() == kObjectBark && target == 3) {
						_app->bagRemove(_app->bagGetClickedObject());
						_app->objectPresentationShow(kObjectTree, 4);
						_app->varSetWord(50000, _app->varGetWord(50000) + 1000);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
					}
				} else {
					_app->bagRemove(_app->bagGetClickedObject());
					_app->objectPresentationShow(kObjectTree, 3);
					_app->varSetWord(50000, _app->varGetWord(50000) + 100);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
				}
			}
			else
			{
				_app->bagRemove(_app->bagGetClickedObject());
				_app->objectPresentationShow(kObjectTree, 2);
				_app->varSetWord(50000, _app->varGetWord(50000) + 10);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
			}
		}
		else
		{
			_app->bagRemove(kObjectFlower);
			_app->objectPresentationShow(kObjectTree, 1);
			_app->varSetWord(50000, _app->varGetWord(50000) + 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
		}

		if (_app->varGetWord(50000) == 1111) {
			_app->varSetWord(50000, 11111);
			_app->objectPresentationShow(kObjectTree, 6);

			handleEvents();

			_app->playMovie("1851");
			_app->rotationSetActive(50502);
			_app->bagAdd(kObjectThread);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
			_app->varSetByte(50012, _app->varGetByte(50012) + 1);

			if (_app->varGetByte(50012) == 2) {
				_app->objectPresentationShow(kObject50700, 0);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 1, 1);
				_app->rotationSetMovabilityOff(50103, 2, 2);
				_app->rotationSetMovabilityOn(50103, 3, 3);
			}

			if (_app->varGetByte(50012) == 4) {
				_app->objectPresentationShow(kObject50700, 1);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 2, 2);
			}
		}
		_app->cursorDelete();
		break;

	case kObjectGolem1:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectGolem1) {
				if (!target) {
					_app->bagRemove(kObjectGolem1);
					_app->objectPresentationShow(kObjectGolem1, 1);
					_app->objectPresentationShow(kObjectBackhead, 0);
					_app->objectPresentationShow(kObjectFronthead, 0);
					_app->objectPresentationShow(kObjectBelly, 0);
					_app->objectPresentationShow(kObjectRightArm, 0);
					_app->objectPresentationShow(kObjectLeftArm, 0);
					_app->objectPresentationShow(kObjectLegs, 0);
					_app->objectPresentationShow(kObjectHeart, 0);
					_app->objectSetAccessibilityOn(kObjectBackhead);
					_app->objectSetAccessibilityOn(kObjectFronthead);
					_app->objectSetAccessibilityOn(kObjectBelly);
					_app->objectSetAccessibilityOn(kObjectRightArm);
					_app->objectSetAccessibilityOn(kObjectLeftArm);
					_app->objectSetAccessibilityOn(kObjectLegs);
					_app->objectSetAccessibilityOn(kObjectHeart, 1, 1);
					_app->objectSetAccessibilityOff(kObjectGolem1, 0, 0);
					_app->puzzleSetMovabilityOff(kPuzzle50400);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
				}
			}
			_app->cursorDelete();
		}
		break;

	case kObject50105:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(50011) == 1) {
			_app->objectPresentationPauseAnimationFrame(kObject50100, 8, 50, 1000, 2);
			_app->varSetByte(50012, 5);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 3.0f);
			_app->objectPresentationShow(kObject50100, 8);
			_app->objectPresentationHide(kObject50100, 7);
		}
		break;

	case kObject50100:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(50012) == 4) {
			_app->rotationSetRolTo(50108, 250.0, 15.0, 85.7f);
			_app->playMovie("1849");
			_app->puzzleSetActive(kPuzzle50100);
			_app->soundStop(51006, 1024);
		}

		if (_app->varGetByte(50012) > 4) {
			_app->playMovie("1850");
			_app->rotationSetAlp(50601, 250.0);
			_app->rotationSetActive(50601);
			_app->soundStop(51006, 1024);
		}
		break;

	case kObjectBark:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(kObjectBark);
		_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
		_app->objectSetAccessibilityOff(kObjectBark);
		break;

	case kObjectCounch1:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != 50000 || _app->varGetByte(50005)) {
				if (_app->bagGetClickedObject() == 50501) {
					if (_app->varGetByte(50005) == 1) {
						_app->bagRemove(kObjectDragonSword);
						_app->rotationSetRolTo(50304, 145.0f, 9.0f, 85.7f);
						_app->varSetByte(50005, 2);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
						_app->rotationSetMovabilityOff(50304, 0, 0);
						_app->rotationSetMovabilityOn(50304, 1, 1);
						_app->objectSetAccessibilityOn(kObjectCounch1, 1, 1);
						_app->objectPresentationHide(kObjectCounch1);

						handleEvents();

						_app->playMovie("1853");
						_app->rotationSetAlp(50304, 190.0);
						_app->rotationSetBet(50304, 15.0);
						_app->rotationSetActive(50304);
						_app->objectPresentationShow(kObjectCounch1, 1);

						if (!_app->varGetByte(50003))
							_app->objectPresentationShow(kObjectCounch1, 4);
					}
				}
				_app->cursorDelete();

			} else {
				_app->objectPresentationShow(kObjectCounch1, 0);
				_app->varSetByte(50005, 1);
				_app->playMovie("1852");
				_app->rotationSetAlp(50304, 190.0);
				_app->rotationSetBet(50304, 15.0);
				_app->rotationSetActive(50304);
				_app->cursorDelete();
			}
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(50005) >= 2) {
				if (_app->varGetByte(50005) >= 2) {
					_app->objectPresentationShow(kObjectCounch1, 5);
					_app->objectSetAccessibilityOff(kObjectCounch1, 0, 0);
					_app->bagAdd(kObjectDragonSword);
				}
			}
			else {
				_app->puzzleSetActive(kPuzzle50304);
			}
			break;

		case 1:
			if (_app->varGetByte(50005) == 2) {
				_app->varSetByte(50005, 3);
				_app->objectPresentationShow(kObjectCounch1, 2);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->objectSetAccessibilityOff(kObjectCounch1, 1, 1);
				_app->bagAdd(kObjectCounch1);
			}
			break;
		}
		break;

	case kObjectPhoenix:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() != 50000) {
				_app->rotationSetActive(50303);
				_app->cursorDelete();
				break;
			}

			_app->objectPresentationShow(kObjectPhoenix, 0);
			_app->objectSetAccessibilityOff(kObjectPhoenix);
			_app->objectSetAccessibilityOn(kObjectBark);
			_app->varSetByte(50003, 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			_app->soundPlay(51013, kSoundLoop);
			_app->rotationSetMovabilityOff(50301, 0, 1);
			_app->rotationSetMovabilityOn(50301, 2, 3);
			_app->rotationSetMovabilityOff(50302, 0, 2);
			_app->rotationSetMovabilityOn(50302, 3, 5);
			_app->rotationSetMovabilityOff(50303, 0, 0);
			_app->rotationSetMovabilityOn(50303, 1, 1);

			switch (_app->varGetByte(50005)) {
			default:
				_app->objectPresentationHide(kObjectCounch1);
				_app->objectPresentationShow(kObjectCounch1, 1);
				break;

			case 0:
			case 1:
				_app->objectPresentationShow(kObjectCounch1, 3);
				break;

			case 3:
				_app->objectPresentationHide(kObjectCounch1);
				_app->objectPresentationShow(kObjectCounch1, 1);
				_app->objectPresentationShow(kObjectCounch1, 2);
				break;
			}

			_app->objectPresentationShow(kObjectPhoenix, 0);
			_app->playMovie("1854");
			_app->varSetByte(50012, _app->varGetByte(50012) + 1);

			if (_app->varGetByte(50012) == 2) {
				_app->objectPresentationShow(kObject50700, 0);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 1, 1);
				_app->rotationSetMovabilityOff(50103, 2, 2);
				_app->rotationSetMovabilityOn(50103, 3, 3);
			}

			if (_app->varGetByte(50012) == 4) {
				_app->objectPresentationShow(kObject50700, 1);
				_app->objectSetAccessibilityOff(kObject50700);
				_app->objectSetAccessibilityOn(kObject50700, 2, 2);
			}

			_app->rotationSetActive(50303);
			_app->cursorDelete();
			break;
		}

		_app->puzzleSetActive(kPuzzle50303);
		break;

	case kObjectRopes:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDragonSword) {
				_app->playMovie("1855");
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->objectPresentationShow(kObjectRopes, 0);
				_app->varSetByte(50001, 1);
				_app->objectSetAccessibilityOff(kObjectRopes);
			}

			_app->cursorDelete();
		}
		break;

	case kObjectCloud:
		if (!_app->bagHasClickedObject())
			break;

		if (_app->bagGetClickedObject() == kObjectCounch1) {
			if (_app->bagHas(kObjectFeather)) {
				_app->playMovie("1856");

				if (_app->varGetByte(50001) == 1) {
					_app->objectPresentationHide(kObjectRopes, 0);
					_app->objectPresentationShow(kObjectRopes, 1);
					_app->varSetByte(50001, 2);
					_app->playMovie("1857");
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
					_app->objectSetAccessibilityOff(kObjectCloud);

					if (_app->varGetByte(50003) == 1) {
						_app->objectPresentationHide(kObjectRopes, 1);
						_app->objectPresentationShow(kObjectRopes, 2);
						_app->varSetByte(50001, 3);
						_app->playMovie("1858");
						_app->bagAdd(kObjectDeadLeaf);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
						_app->varSetByte(50012, _app->varGetByte(50012) + 1);

						if (_app->varGetByte(50012) == 2) {
							_app->objectPresentationShow(kObject50700, 0);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 1, 1);
							_app->rotationSetMovabilityOff(50103, 2, 2);
							_app->rotationSetMovabilityOn(50103, 3, 3);
						}

						if (_app->varGetByte(50012) == 4) {
							_app->objectPresentationShow(kObject50700, 1);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 2, 2);
						}
					}
				}
			} else {
				_app->playMovie("1859", 0.0);
			}
		}

		_app->cursorDelete();
		break;
	}
}

void ZoneWARing::onButtonUp2(ObjectId objectId, uint32 index, Id, uint32, const Common::Point &) {
	switch (objectId) {
	default:
		break;

	case kObjectMagnet:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() >= kObject50451
			 && _app->bagGetClickedObject() <= kObject50457) {

				_app->objectPresentationSetImageCoordinatesOnPuzzle(_app->bagGetClickedObject(), 0, Common::Point((int16)(53 * index / 10 + 162), (int16)(37 * index % 10 + 115)));
				_app->varSetDword(index + 51000, _app->bagGetClickedObject());
				_app->objectSetAccessibilityOn(_app->bagGetClickedObject(), index / 10 + 1 + 7 * index % 10, index / 10 + 1 + 7 * index % 10);
				_app->objectSetAccessibilityOff(kObjectMagnet, index / 10 + 7 * index % 10, index / 10 + 7 * index % 10);
				_app->objectPresentationShow(_app->bagGetClickedObject());

				if (_app->varGetDword(51030) == 50451
				 && _app->bagGetClickedObject() == kObject50451)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51061) == 50452
				 && _app->bagGetClickedObject() == kObject50452)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51001) == 50453
				 && _app->bagGetClickedObject() == kObject50453)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51033) == 50454
				 && _app->bagGetClickedObject() == kObject50454)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51026) == 50455
				 && _app->bagGetClickedObject() == kObject50455)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51046) == 50456
				 && _app->bagGetClickedObject() == kObject50456)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				if (_app->varGetDword(51042) == 50457
				 && _app->bagGetClickedObject() == kObject50457)
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);

				_app->bagRemove(_app->bagGetClickedObject());

				if (_app->varGetDword(51030) == 50451
				 && _app->varGetDword(51061) == 50452
				 && _app->varGetDword(51001) == 50453
				 && _app->varGetDword(51033) == 50454
				 && _app->varGetDword(51026) == 50455
				 && _app->varGetDword(51046) == 50456) {

					_app->objectSetAccessibilityOn(kObject50457, 0, 0);

					if (_app->varGetDword(51042) == 50457) {
						_app->playMovie("1867");
						_app->rotationSetMovabilityOff(50402, 2, 2);
						_app->bagAdd(kObjectFeather);
						_app->varSetFloat(90008, _app->varGetFloat(90008) + 2.0f);
						_app->rotationSetAlp(50402, 180.0f);
						_app->rotationSetBet(50402, 0.3f);
						_app->rotationSetActive(50402);
						_app->varSetByte(50012, _app->varGetByte(50012) + 1);

						if (_app->varGetByte(50012) == 2) {
							_app->objectPresentationShow(kObject50700, 0);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 1, 1);
							_app->rotationSetMovabilityOff(50103, 2, 2);
							_app->rotationSetMovabilityOn(50103, 3, 3);
						}

						if (_app->varGetByte(50012) == 4) {
							_app->objectPresentationShow(kObject50700, 1);
							_app->objectSetAccessibilityOff(kObject50700);
							_app->objectSetAccessibilityOn(kObject50700, 2, 2);
						}
					}
				}
			}

			_app->cursorDelete();
		}

		_app->setField74(false);
		break;

	case kObject50451:
	case kObject50452:
	case kObject50453:
	case kObject50454:
	case kObject50455:
	case kObject50456:
	case kObject50457:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() < kObject50451
			 || _app->bagGetClickedObject() > kObject50457
			 || (index && !_app->varGetDword(index + 50999))) {
				_app->setField74(false);
				_app->cursorDelete();
				break;
			}

			_app->objectPresentationSetImageOriginalCoordinatesOnPuzzle(_app->bagGetClickedObject(), 0);

			_app->objectSetAccessibilityOn(_app->bagGetClickedObject(), 0, 0);
			_app->objectPresentationShow(_app->bagGetClickedObject());
			_app->bagRemove(_app->bagGetClickedObject());
			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		switch (index) {
		default: {
			if (_app->varGetDword(51030) == 50451
				&& objectId == kObject50451)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51061) == 50452
				&& objectId == kObject50452)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51001) == 50453
				&& objectId == kObject50453)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51033) == 50454
				&& objectId == kObject50454)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51026) == 50455
				&& objectId == kObject50455)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51046) == 50456
				&& objectId == kObject50456)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			if (_app->varGetDword(51042) == 50457
				&& objectId == kObject50457)
				_app->varSetFloat(90008, _app->varGetFloat(90008) - 1.0f);

			_app->varSetDword(index + 50999, 0);

			uint32 idx = (index - 1) / 10 + 7 * (index - 1) % 10;
			_app->objectSetAccessibilityOff(objectId, idx + 1, idx + 1);
			_app->objectSetAccessibilityOn(kObjectMagnet, idx, idx);
			_app->objectPresentationHide(objectId);
			_app->bagAdd(objectId);
			}
			break;

		case 0:
			_app->objectPresentationHide(objectId);
			_app->bagAdd(objectId);
			_app->objectSetAccessibilityOff(objectId, index, index);
			break;
		}
		break;

	case kObjectFronthead:
	case kObjectBackhead:
	case kObjectBelly:
	case kObjectRightArm:
	case kObjectLeftArm:
	case kObjectLegs:
	case kObjectHeart:
		if (!_app->bagHasClickedObject()) {
			if (!index && !_app->bagHas(objectId)) {
				_app->objectPresentationHide(objectId, 0);
				_app->bagAdd(objectId);
				break;
			}

			_app->setField74(false);
			break;
		}

		if (_app->bagGetClickedObject().id() == objectId && index > 0) {
			_app->objectPresentationShow(objectId, 1);
			_app->objectSetAccessibilityOff(objectId, 0, 1);
			_app->varSetFloat(90008, _app->varGetFloat(90008) + 1.0f);
			_app->varSetDword(50000, (int32)index + _app->varGetDword(50000));

			if (_app->varGetDword(50000) == 7654321) {
				_app->playMovie("1865");
				_app->playMovie("1866");
				_app->rotationSetMovabilityOff(50401, 1, 1);
				_app->rotationSetMovabilityOn(50401, 2, 2);
				_app->rotationSetMovabilityOff(50402, 1, 1);
				_app->rotationSetMovabilityOn(50402, 2, 2);
				_app->objectPresentationShow(kObjectGolem1, 0);
				_app->rotationSetAlp(50402, 180.0f);
				_app->rotationSetBet(50402, 0.3f);
				_app->rotationSetActive(50402);
			} else {
				if (_app->varGetDword(50000) == 654321)
					_app->objectSetAccessibilityOn(kObjectHeart);
			}

			_app->bagRemove(objectId);
			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		if (_app->bagGetClickedObject() != kObjectFronthead
		 && _app->bagGetClickedObject() != kObjectBackhead
		 && _app->bagGetClickedObject() != kObjectBelly
		 && _app->bagGetClickedObject() != kObjectRightArm
		 && _app->bagGetClickedObject() != kObjectLeftArm
		 && _app->bagGetClickedObject() != kObjectLegs
		 && _app->bagGetClickedObject() != kObjectHeart) {
		 	_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		_app->objectPresentationShow(_app->bagGetClickedObject(), 0);
		_app->bagRemove(_app->bagGetClickedObject());
		_app->setField74(false);
		_app->cursorDelete();
		break;

	case kObjectInk:
	case kObjectPaper:
	case kObjectStylet:
	case kObjectInkedStylet:
		if (_app->bagHasClickedObject()) {
			switch (index) {
			default:
				break;

			case 0:
			case 1:
			case 2:
				if (_app->bagGetClickedObject().id() == objectId) {
					_app->bagRemove(objectId);
					_app->objectPresentationShow(kObject50100, index);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;

			case 8:
				if (_app->bagGetClickedObject() == kObjectPaper) {
					_app->objectPresentationShow(kObject50100, 3);
					_app->bagRemove(kObjectPaper);
					_app->objectSetAccessibilityOff(kObjectPaper, 0, 0);
					_app->varSetWord(50103, 102);
					_app->setField74(false);
					_app->cursorDelete();
				}

				if (_app->bagGetClickedObject() == kObjectInkedStylet
				 && _app->varGetWord(50103) > 0) {
					_app->objectPresentationShow(kObject50100, 5);
					_app->bagRemove(kObjectInkedStylet);
					_app->varSetWord(50105, 104);
					_app->objectPresentationPauseAnimationFrame(kObject50100, 7, 30, 10000, 2);
					_app->objectPresentationShow(kObject50100, 7);
					_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;

			case 9:
				if (_app->bagGetClickedObject() == kObjectInk
				 && !_app->varGetWord(50101)) {
					_app->objectPresentationShow(kObject50100, 4);
					_app->bagRemove(kObjectInk);
					_app->objectSetAccessibilityOff(kObjectInk, 0, 0);
					_app->varSetWord(50101, 101);
					_app->setField74(false);
					_app->cursorDelete();
				}

				if (_app->bagGetClickedObject() == kObjectStylet
				 && _app->varGetWord(50101) > 0) {
					_app->objectPresentationHide(kObject50100, 4);
					_app->objectPresentationShow(kObject50100, 6);
					_app->objectSetAccessibilityOff(kObjectStylet, 0, 0);
					_app->bagRemove(kObjectStylet);
					_app->varSetWord(50102, 103);
					_app->setField74(false);
					_app->cursorDelete();
				}
				break;
			}

			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		switch (index) {
		default:
			break;

		case 0:
		case 1:
		case 2:
			if (!_app->bagHas(objectId)) {
				_app->bagAdd(objectId);
				_app->objectPresentationHide(kObject50100, index);
			}
			break;

		case 9:
			if (_app->varGetWord(50102) > 0
			&& !_app->bagHas(kObjectInkedStylet)) {
				_app->objectPresentationHide(kObject50100, 6);
				_app->bagAdd(kObjectInkedStylet);
			}
			break;
		}
		_app->setField74(false);
		break;
	}
}

void ZoneWARing::onAnimation(uint32 type, Id animationId, const Common::String &, uint32, uint32) {
	switch (animationId) {
	default:
		break;

	case 50003:
		if (type == 1) {
			_app->varSetByte(50011, 1);
		} else if (type == 2) {
			_app->varSetByte(50011, 0);
			_app->objectPresentationHide(kObject50100, 3);
			_app->objectPresentationHide(kObject50100, 4);
			_app->objectPresentationHide(kObject50100, 5);
			_app->objectPresentationHide(kObject50100, 6);
			_app->objectPresentationShow(kObject50100, 0);
			_app->objectPresentationShow(kObject50100, 1);
			_app->objectPresentationShow(kObject50100, 2);
			_app->varSetWord(50101, 0);
			_app->varSetWord(50102, 0);
			_app->varSetWord(50103, 0);
			_app->varSetWord(50104, 0);
			_app->varSetWord(50105, 0);
			_app->objectSetAccessibilityOn(kObjectInk);
			_app->objectSetAccessibilityOn(kObjectPaper);
			_app->objectSetAccessibilityOn(kObjectStylet);
			_app->objectSetAccessibilityOn(kObjectInkedStylet);

		}
		break;

	case 50004:
		if (type == 2) {
			_app->objectPresentationHide(kObject50100);
			_app->rotationSetAlp(50601, 250.0);
			_app->rotationSetActive(50601);
			_app->playMovie("1872");
		}
		break;
	}
}

void ZoneWARing::onAnimationNextFrame(Id animationId, const Common::String &, uint32 frame, uint32) {
	if (animationId == 50001) {
		if (_app->varGetByte(50007) == 1 && _app->varGetByte(50009) == 1) {
			if (frame == 35 || frame == 12) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 6);
				_app->objectPresentationHide(kObject50600, 7);
				_app->soundStop(50018, 1024);

				if (_app->varGetByte(50012) == 6) {
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 5.0f);
					_app->varSetByte(50012, 5);
				}
			}
		}

		if (_app->varGetByte(50008) == 1 && _app->varGetByte(50010) == 1) {
			if (frame == 49 || frame == 24) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 6);
				_app->objectPresentationShow(kObject50600, 7);
				_app->soundStop(50018, 1024);

				if (_app->varGetByte(50012) == 6) {
					_app->varSetFloat(90008, _app->varGetFloat(90008) - 5.0f);
					_app->varSetByte(50012, 5);
				}
			}
		}

		if (_app->varGetByte(50008) == 1 && _app->varGetByte(50009) == 1) {
			if (frame == 17 || frame == 41) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 7);
				_app->objectPresentationShow(kObject50600, 6);
				_app->soundStop(50018, 1024);
				_app->varSetByte(50012, 6);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			}
		}

		if (_app->varGetByte(50007) == 1 && _app->varGetByte(50010) == 1) {
			if (frame == 4 || frame == 30) {
				_app->objectPresentationPauseAnimation(kObject50600, 0);
				_app->objectPresentationHide(kObject50600, 1);
				_app->objectPresentationHide(kObject50600, 7);
				_app->objectPresentationShow(kObject50600, 6);
				_app->soundStop(50018, 1024);
				_app->varSetByte(50012, 6);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
			}
		}
	}
}

void ZoneWARing::onSound(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 50021:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1875", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1873", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1874", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51002);
		_app->soundPlay(50022);
		break;

	case 50022:
		switch (_app->getCurrentLanguage()) {
		default:
			_app->playMovieChannel("1878", _app->getLanguageChannel());
			break;

		case kLanguageItalian:
		case kLanguageSpanish:
		case kLanguageDutch:
			_app->playMovieChannel("1876", _app->getLanguageChannel());
			break;

		case kLanguageSwedish:
			_app->playMovieChannel("1877", _app->getLanguageChannel());
			break;
		}

		_app->puzzleSetActive(kPuzzle51003);
		_app->soundPlay(51008, kSoundLoop);
		_app->soundPlay(50026);
		break;

	case 50026:
		_app->puzzleSetActive(kPuzzle51004);
		_app->soundPlay(50027);
		break;

	case 50027:
		_app->puzzleSetActive(kPuzzle51005);
		_app->soundPlay(50028);
		break;

	case 50028:
		_app->puzzleSetActive(kPuzzle51006);
		_app->soundPlay(50029);
		break;

	case 50029:
		_app->puzzleSetActive(kPuzzle51007);
		_app->soundPlay(50030);
		break;

	case 50030:
		_app->puzzleSetActive(kPuzzle51008);
		_app->soundPlay(50031);
		break;

	case 50031:
		_app->puzzleSetActive(kPuzzle51009);
		_app->soundPlay(50032);
		break;

	case 50032:
		_app->puzzleSetActive(kPuzzle51010);
		_app->soundPlay(50033);
		break;

	case 50033:
		_app->puzzleSetActive(kPuzzle51011);
		_app->soundPlay(50034);
		break;

	case 50034:
		_app->puzzleSetActive(kPuzzle51012);
		_app->soundPlay(50035);
		break;

	case 50035:
		_app->puzzleSetActive(kPuzzle51013);
		_app->soundPlay(50036);
		break;

	case 50036:
		_app->playMovie("1879");
		_app->timerStopAll();
		_app->soundStopAll(1024);
		_app->onSwitchZone(kZoneAS, 4);
		break;

	case 50001:
		_app->puzzleSetActive(kPuzzle50001);
		_app->soundPlay(50002);

		break;
	case 50002:
		_app->rotationSetActive(50001);
		_app->soundPlay(50003);
		break;

	case 50003:
		_app->soundPlay(51006, kSoundLoop);
		break;

	case 50009:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50010);
		break;

	case 50010:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50011);
		break;

	case 50011:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50012);
		break;

	case 50012:
		_app->puzzleSetActive(kPuzzle50701);
		_app->soundPlay(50013);
		break;

	case 50013:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50014);
		break;

	case 50014:
		_app->puzzleSetActive(kPuzzle50702);
		_app->soundPlay(50015);
		break;

	case 50015:
		_app->puzzleSetActive(kPuzzle50703);
		_app->soundPlay(50016);
		break;

	case 50016:
		_app->objectSetAccessibilityOff(kObject50700);
		_app->rotationSetActive(50701);
		break;
	}
}

void ZoneWARing::onBeforeRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityRotationToRotation)
		return;

	switch (movabilityFrom){
	default:
		break;

	case 50001:
		if (movabilityTo == 50101)
			if (!_app->soundIsPlaying(51006))
				_app->soundPlay(51006, kSoundLoop);
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51007, kSoundLoop);
		}
		break;

	case 50304:
		if (movabilityTo == 50302) {
			if (_app->varGetByte(50003)) {
				if (_app->varGetByte(50005) >= 2)
					_app->playMovie("1871");
				else
					_app->playMovie("1870");
			} else if (_app->varGetByte(50005)) {
				_app->playMovie("1869");
			} else {
				_app->playMovie("1868");
			}
		}
		break;
	}
}

void ZoneWARing::onAfterRide(Id movabilityFrom, Id movabilityTo, uint32, Id, MovabilityType movabilityType) {
	if (movabilityType != kMovabilityRotationToRotation)
		return;

	switch (movabilityFrom) {
	default:
		break;

	case 50201:
		if (movabilityTo == 50102) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51002, kSoundLoop);
			_app->soundPlay(51012, kSoundLoop);
		}

		if (_app->varGetByte(50001) == 2) {
			if (_app->varGetByte(50003) == 1) {
				_app->objectPresentationHide(kObjectRopes, 1);
				_app->objectPresentationShow(kObjectRopes, 2);
				_app->varSetByte(50001, 3);
				_app->playMovie("1858");
				_app->bagAdd(kObjectDeadLeaf);
				_app->varSetFloat(90008, _app->varGetFloat(90008) + 5.0f);
				_app->varSetByte(50012, _app->varGetByte(50012) + 1);

				if (_app->varGetByte(50012) == 2) {
					_app->objectPresentationShow(kObject50700, 0);
					_app->objectSetAccessibilityOff(kObject50700);
					_app->objectSetAccessibilityOn(kObject50700, 1, 1);
					_app->rotationSetMovabilityOff(50103, 2, 2);
					_app->rotationSetMovabilityOn(50103, 3, 3);
				}

				if (_app->varGetByte(50012) == 4) {
					_app->objectPresentationShow(kObject50700, 1);
					_app->objectSetAccessibilityOff(kObject50700);
					_app->objectSetAccessibilityOn(kObject50700, 2, 2);
				}
			}
		}
		break;

	case 50102:
		if (movabilityTo == 50201) {
			_app->soundStop(51002, 1024);
			_app->soundStop(51012, 1024);
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50301:
		if (movabilityTo == 50104) {
			_app->soundStop(51006, 1024);
			_app->soundPlay(51004, kSoundLoop);

			if (_app->varGetByte(50003) > 0)
				_app->soundPlay(51013, kSoundLoop);
		}
		break;

	case 50104:
		if (movabilityTo == 50301) {
			_app->soundStop(51004, 1024);
			_app->soundStop(51013, 1024);
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50106:
		if (movabilityTo == 50501) {
			_app->soundStop(51001, 1024);
			_app->soundStop(51011, 1024);
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50103:
		if (movabilityTo == 50701) {
			_app->soundStop(51007, 1024);
			_app->soundPlay(51006, kSoundLoop);
		}
		break;

	case 50108:
		if (movabilityTo == 50601)
			_app->soundPlay(51006, kSoundLoop);
		break;
	}
}

} // End of namespace Ring
