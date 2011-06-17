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

#include "ring/game/ring/ring_zonerh.h"

#include "ring/game/ring/ring_application.h"
#include "ring/game/ring/ring_shared.h"

#include "ring/debug.h"
#include "ring/ring.h"
#include "ring/helpers.h"

using namespace RingGame;

namespace Ring {

ZoneRHRing::ZoneRHRing(ApplicationRing *application) : _app(application) {
	_disableTimerRH = false;
	_frameNumberTimerRH = 0;
}

ZoneRHRing::~ZoneRHRing() {
	// Zero-out passed pointers
	_app = NULL;
}

void ZoneRHRing::onInit() {
	debugC(1, kRingDebugLogic, "Initializing Alberich zone (RH)...");

	_app->setZoneAndEnableBag(kZoneRH);

	_app->puzzleAdd(kPuzzle22001);
	_app->puzzleAddBackgroundImage(kPuzzle22001, "TR_NI_RH_BP01.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle22002);
	_app->puzzleAddBackgroundImage(kPuzzle22002, "TR_NI_RH_BP02.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle22003);
	_app->puzzleAddBackgroundImage(kPuzzle22003, "TR_NI_RH_BP03.BMP", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20000);
	_app->puzzleAddBackgroundImage(kPuzzle20000, "RHS00N01P01S02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20011);
	_app->puzzleAddBackgroundImage(kPuzzle20011, "RHS00N01P01L00.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20021);
	_app->puzzleAddBackgroundImage(kPuzzle20021, "RHS00N02P01L00.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20031);
	_app->puzzleAddBackgroundImage(kPuzzle20031, "RHS00N03P01L00.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20202);
	_app->puzzleAddBackgroundImage(kPuzzle20202, "RHS02N01P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20203);
	_app->puzzleAddBackgroundImage(kPuzzle20203, "RHS02N01P02DI01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20204);
	_app->puzzleAddBackgroundImage(kPuzzle20204, "RHS02N02P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20301);
	_app->puzzleAddBackgroundImage(kPuzzle20301, "RHS03N01P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20302);
	_app->puzzleAddBackgroundImage(kPuzzle20302, "RHS03N02P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20303);
	_app->puzzleAddBackgroundImage(kPuzzle20303, "RHS03N03P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20401);
	_app->puzzleAddBackgroundImage(kPuzzle20401, "RHS04N01P01L01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20501);
	_app->puzzleAddBackgroundImage(kPuzzle20501, "RHS05N01P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20502);
	_app->puzzleAddBackgroundImage(kPuzzle20502, "RHS05N01P02.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20503);
	_app->puzzleAddBackgroundImage(kPuzzle20503, "RHS05N02P01.bmp", Common::Point(0, 16), true);
	_app->puzzleAdd(kPuzzle20701);
	_app->puzzleAddBackgroundImage(kPuzzle20701, "RHS07N01P01L01.bmp", Common::Point(0, 16), true);
	_app->rotationAdd(20010, "RHS00N01", 0, 0);
	_app->rotationAdd(20020, "RHS00N02", 0, 0);
	_app->rotationAdd(20030, "RHS00N03", 0, 0);
	_app->rotationAdd(20101, "RHS01N01", 0, 2);
	_app->rotationSetCompressionBufferLength(20101, 1300000);
	_app->rotationAdd(20201, "RHS02N01", 0, 1);
	_app->rotationAdd(20202, "RHS02N02", 0, 1);
	_app->rotationAdd(20203, "RHS02N03", 0, 1);
	_app->rotationAdd(20301, "RHS03N01", 0, 2);
	_app->rotationSetCompressionBufferLength(20301, 1300000);
	_app->rotationAdd(20302, "RHS03N02", 0, 0);
	_app->rotationAdd(20303, "RHS03N03", 0, 0);
	_app->rotationAdd(20304, "RHS03N04", 0, 1);
	_app->rotationAdd(20305, "RHS03N05", 0, 1);
	_app->rotationAdd(20401, "RHS04N01", 0, 1);
	_app->rotationAdd(20402, "RHS04N02", 0, 1);
	_app->rotationAdd(20403, "RHS04N03", 0, 1);
	_app->rotationAdd(20501, "RHS05N01", 0, 1);
	_app->rotationAdd(20503, "RHS05N03", 0, 1);
	_app->rotationAdd(20504, "RHS05N04", 0, 0);
	_app->rotationAdd(20601, "RHS06N01", 0, 0);
	_app->rotationAdd(20701, "RHS07N01", 0, 0);
	_app->rotationSetJugOn(20701, 10.0f, 1.0);
	_app->rotationAddMovabilityToPuzzle(20010, kPuzzle20011, "1708", Common::Rect(274, -289, 836, 102), true, kCursorMove, 0);
	_app->rotationSetMovabilityToPuzzle(20010, 0, 1.5f, -4.3f, 79.4f, 0, 0);
	_app->rotationAddMovabilityToRotation(20010, 20020, "1709", Common::Rect(274, -289, 836, 102), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(20010, 1, 1.5f, -4.3f, 79.4f, 0, 0, 1.5f, -4.3f, 79.4f);
	_app->rotationAddMovabilityToPuzzle(20020, kPuzzle20021, "1713", Common::Rect(274, -289, 836, 102), true, kCursorMove, 0);
	_app->rotationSetMovabilityToPuzzle(20020, 0, 1.5f, -4.3f, 79.4f, 0, 0);
	_app->rotationAddMovabilityToRotation(20020, 20030, "1709", Common::Rect(274, -289, 836, 102), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(20020, 1, 1.5f, -4.3f, 79.4f, 0, 0, 1.5f, -4.3f, 79.4f);
	_app->rotationAddMovabilityToRotation(20020, 20010, "1709", Common::Rect(2074, -289, 2636, 102), true, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(20020, 2, 181.5f, -4.3f, 79.4f, 0, 0, 181.5f, -4.3f, 79.4f);
	_app->rotationAddMovabilityToPuzzle(20030, kPuzzle20031, "1717", Common::Rect(274, -289, 836, 102), true, kCursorMove, 0);
	_app->rotationSetMovabilityToPuzzle(20030, 0, 1.5f, -4.3f, 79.4f, 0, 0);
	_app->rotationAddMovabilityToRotation(20030, 20101, "1718", Common::Rect(0, -289, 836, 102), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(20030, 1, 325.0f, -4.3f, 79.4f, 0, 2, 342.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20030, 20020, "1709", Common::Rect(2074, -289, 2636, 102), true, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(20030, 2, 181.5f, -4.3f, 79.4f, 0, 0, 181.5f, -4.3f, 79.4f);
	_app->rotationAddMovabilityToRotation(20101, 20030, "1709", Common::Rect(2239, -62, 2902, 240), true, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(20101, 0, 215.0f, 0.3f, 85.7f, 0, 2, 181.5f, -4.3f, 79.4f);
	_app->rotationAddMovabilityToRotation(20101, 20201, "1695", Common::Rect(82, -97, 362, 268), false, kCursorMove, 1);
	_app->rotationSetMovabilityToRotation(20101, 1, 215.0f, 0.3f, 85.7f, 0, 2, 0, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20201, 20202, "1722", Common::Rect(356, -162, 553, 238), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20201, 0, 0, 0.3f, 85.7f, 0, 2, 0, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20201, 20101, "1723", Common::Rect(2098, -130, 2371, 221), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20201, 1, 185.0f, 0.3f, 85.7f, 0, 0, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20202, 20203, "1724", Common::Rect(346, -88, 569, 191), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20202, 0, 0, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20202, 20203, "1725", Common::Rect(346, -88, 569, 191), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20202, 1, 0, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20202, 20201, "1726", Common::Rect(1921, -251, 2515, 450), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20202, 2, 180.0f, 0.3f, 85.7f, 0, 0, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20203, 20202, "1727", Common::Rect(1218, -88, 1499, 204), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20203, 0, 90.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20203, 20202, "1728", Common::Rect(1218, -88, 1499, 204), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20203, 1, 90.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20203, 20301, "1707", Common::Rect(3300, -560, 3600, 600), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20203, 2, 270.0f, 0.3f, 85.7f, 0, 2, 42.0f, 23.0f, 85.7f);
	_app->rotationAddMovabilityToRotation(20203, 20301, "1707", Common::Rect(0, -600, 50, 560), false, kCursorMove, 0); // Updated to valid rect
	_app->rotationSetMovabilityToRotation(20203, 3, 270.0f, 0.3f, 85.7f, 0, 2, 42.0f, 23.0f, 85.7f);
	_app->rotationAddMovabilityToRotation(20304, 20305, "1729", Common::Rect(346, -88, 569, 191), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20304, 0, 0, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20304, 20305, "1730", Common::Rect(346, -88, 569, 191), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20304, 1, 0, 0.3f, 85.7f, 0, 2, 90.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20304, 20303, "1731", Common::Rect(1933, -350, 2634, 430), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20304, 2, 180.0f, 0.3f, 85.7f, 0, 0, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20305, 20304, "1732", Common::Rect(2009, -55, 2369, 320), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20305, 0, 90.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20305, 20304, "1733", Common::Rect(2009, -55, 2369, 320), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20305, 1, 90.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20305, 20401, "1734", Common::Rect(680, -270, 1430, 530), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20305, 2, 270.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20402, 20403, "1735", Common::Rect(346, -88, 569, 191), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20402, 0, 1.0f, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20402, 20403, "1736", Common::Rect(346, -88, 569, 191), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20402, 1, 1.0f, 0.3f, 85.7f, 0, 2, 270.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20403, 20402, "1737", Common::Rect(1209, -68, 1474, 207), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20403, 0, 90.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20403, 20402, "1738", Common::Rect(1209, -68, 1474, 207), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20403, 1, 90.0f, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20403, 20501, "1739", Common::Rect(3000, -600, 3170, 460), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20403, 2, 270.0f, 0.3f, 85.7f, 0, 2, 10.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20503, 20501, "1740", Common::Rect(1933, -350, 2634, 430), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20503, 0, 180.0f, 0.3f, 85.7f, 0, 0, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20503, 20504, "1741", Common::Rect(346, -88, 569, 191), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20503, 1, 0, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20504, 20503, "1742", Common::Rect(263, -186, 717, 218), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20504, 0, 0, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20504, 20503, "1743", Common::Rect(263, -186, 717, 218), false, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20504, 1, 0, 0.3f, 85.7f, 0, 2, 180.0f, 0.3f, 85.7f);
	_app->rotationAddMovabilityToRotation(20504, 20601, "1744", Common::Rect(2125, -151, 2355, 570), true, kCursorMove, 0);
	_app->rotationSetMovabilityToRotation(20504, 2, 270.0f, 0.3f, 85.7f, 0, 2, 0, 0.3f, 85.7f);
	_app->objectAdd(kObject22000, "", "", 1);
	_app->objectAddPresentation(kObject22000);
	_app->objectPresentationAddAnimationToPuzzle(kObject22000, 0, kPuzzle22001, "TR_NI_RH_BP01S01", kImageTypeBMP, Common::Point(239, 208), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject22000);
	_app->objectPresentationAddAnimationToPuzzle(kObject22000, 1, kPuzzle22002, "TR_NI_RH_BP02S01", kImageTypeBMP, Common::Point(187, 94), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject22000);
	_app->objectPresentationAddAnimationToPuzzle(kObject22000, 2, kPuzzle22003, "TR_NI_RH_BP03S01", kImageTypeBMP, Common::Point(207, 134), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject21003, "", "", 1);
	_app->objectAddPresentation(kObject21003);
	_app->objectPresentationAddImageToRotation(kObject21003, 0, 20203, 0);
	_app->objectAddPresentation(kObject21003);
	_app->objectPresentationAddImageToRotation(kObject21003, 1, 20305, 0);
	_app->objectAddPresentation(kObject21003);
	_app->objectPresentationAddImageToRotation(kObject21003, 2, 20403, 0);
	_app->objectAdd(kObject21001, "", "", 1);
	_app->objectAddPresentation(kObject21001);
	_app->objectPresentationAddAnimationToPuzzle(kObject21001, 0, kPuzzle20000, "RHS00N01P01S02", kImageTypeBMP, Common::Point(215, 155), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20001, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject20001, kPuzzle20011, Common::Rect(339, 22, 453, 396), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject20001, kPuzzle20011, Common::Rect(  0, 16, 639, 463), false, kCursorMove, 1);
	_app->objectAddPresentation(kObject20001);
	_app->objectPresentationAddImageToPuzzle(kObject20001, 0, kPuzzle20011, "RHS00N01P01S01SD.0001.tga", Common::Point(320, 16), true, kDrawType3, 997);
	_app->objectAddPresentation(kObject20001);
	_app->objectPresentationAddAnimationToPuzzle(kObject20001, 1, kPuzzle20011, "RHS00N01P01s01sd", kImageTypeBMP, Common::Point(334, 16), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject20001);
	_app->objectPresentationAddAnimationToPuzzle(kObject20001, 2, kPuzzle20011, "RHS00N01P01a01_a", kImageTypeBMP, Common::Point(334, 16), kDrawType1, 999, 5, 12.5f, 6);
	_app->objectAddPresentation(kObject20001);
	_app->objectPresentationAddAnimationToPuzzle(kObject20001, 3, kPuzzle20011, "RHS00N01P01a01_a", kImageTypeBMP, Common::Point(334, 16), kDrawType1, 999, 5, 12.5f, 10);
	_app->objectAddPresentation(kObject20001);
	_app->objectPresentationAddImageToPuzzle(kObject20001, 4, kPuzzle20011, "RHS00N01P01S01SD.0001.BMP", Common::Point(334, 16), true, kDrawType1, 998);
	_app->objectAdd(kObject20002, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject20002, kPuzzle20021, Common::Rect(204, 99, 314, 436), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject20002, kPuzzle20021, Common::Rect(  0, 16, 639, 463), false, kCursorMove, 1);
	_app->objectAddPresentation(kObject20002);
	_app->objectPresentationAddImageToPuzzle(kObject20002, 0, kPuzzle20021, "RHS00N02P01S01SD.0001.tga", Common::Point(170, 86), true, kDrawType3, 997);
	_app->objectAddPresentation(kObject20002);
	_app->objectPresentationAddAnimationToPuzzle(kObject20002, 1, kPuzzle20021, "RHS00N02P01s01sd", kImageTypeBMP, Common::Point(199, 81), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject20002);
	_app->objectPresentationAddAnimationToPuzzle(kObject20002, 2, kPuzzle20021, "RHS00N02P01a01_a", kImageTypeBMP, Common::Point(199, 81), kDrawType1, 999, 5, 12.5f, 6);
	_app->objectAddPresentation(kObject20002);
	_app->objectPresentationAddAnimationToPuzzle(kObject20002, 3, kPuzzle20021, "RHS00N02P01a01_a", kImageTypeBMP, Common::Point(199, 81), kDrawType1, 999, 5, 12.5f, 10);
	_app->objectAddPresentation(kObject20002);
	_app->objectPresentationAddImageToPuzzle(kObject20002, 4, kPuzzle20021, "RHS00N02P01S01SD.0001.BMP", Common::Point(199, 81), true, kDrawType1, 998);
	_app->objectAdd(kObject20003, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObject20003, kPuzzle20031, Common::Rect(85, 347, 471, 420), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObject20003, kPuzzle20031, Common::Rect( 0,  16, 639, 463), false, kCursorMove, 1);
	_app->objectAddPresentation(kObject20003);
	_app->objectPresentationAddImageToPuzzle(kObject20003, 0, kPuzzle20031, "RHS00N03P01S01SD.0001.bmp", Common::Point(0, 16), true, kDrawType1, 997);
	_app->objectAddPresentation(kObject20003);
	_app->objectPresentationAddAnimationToPuzzle(kObject20003, 1, kPuzzle20031, "RHS00N03P01s01sd", kImageTypeBMP, Common::Point(62, 353), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject20003);
	_app->objectPresentationAddAnimationToPuzzle(kObject20003, 2, kPuzzle20031, "RHS00N03P01a01_a", kImageTypeBMP, Common::Point(60, 351), kDrawType1, 999, 5, 12.5f, 6);
	_app->objectAddPresentation(kObject20003);
	_app->objectPresentationAddAnimationToPuzzle(kObject20003, 3, kPuzzle20031, "RHS00N03P01a01_a", kImageTypeBMP, Common::Point(60, 351), kDrawType1, 999, 5, 12.5f, 10);
	_app->objectAddPresentation(kObject20003);
	_app->objectPresentationAddImageToPuzzle(kObject20003, 4, kPuzzle20031, "RHS00N03P01S01SD1.0001.BMP", Common::Point(62, 353), true, kDrawType1, 998);
	_app->objectAdd(kObjectKeyIndifference, "Indifference", "RH_Key1", 9);
	_app->objectAddBagAnimation(kObjectKeyIndifference, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectKeyIndifference, Common::Point(2, 43), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectKeyIndifference, Common::Point(2, 43), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectKeyIndifference, kPuzzle20011, Common::Rect(288, 284, 362, 422), false, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectKeyIndifference);
	_app->objectPresentationAddAnimationToPuzzle(kObjectKeyIndifference, 0, kPuzzle20011, "RHS00N01P01SA01", kImageTypeTGA, Common::Point(270, 286), kDrawType3, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectKeyMistrust, "Mistrust", "RH_Key1", 9);
	_app->objectAddBagAnimation(kObjectKeyMistrust, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectKeyMistrust, Common::Point(2, 43), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectKeyMistrust, Common::Point(2, 43), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectKeyMistrust, kPuzzle20021, Common::Rect(288, 284, 362, 422), false, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectKeyMistrust);
	_app->objectPresentationAddAnimationToPuzzle(kObjectKeyMistrust, 0, kPuzzle20021, "RHS00N01P01SA01", kImageTypeTGA, Common::Point(270, 286), kDrawType3, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectKeySelfishness, "Selfishness", "RH_Key1", 9);
	_app->objectAddBagAnimation(kObjectKeySelfishness, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectKeySelfishness, Common::Point(2, 43), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectKeySelfishness, Common::Point(2, 43), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectKeySelfishness, kPuzzle20031, Common::Rect(288, 284, 362, 422), false, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectKeySelfishness);
	_app->objectPresentationAddAnimationToPuzzle(kObjectKeySelfishness, 0, kPuzzle20031, "RHS00N01P01SA01", kImageTypeTGA, Common::Point(270, 286), kDrawType3, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectKeyDisgust, "Disgust", "RH_Key1", 9);
	_app->objectAddBagAnimation(kObjectKeyDisgust, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectKeyDisgust, Common::Point(2, 43), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectKeyDisgust, Common::Point(2, 43), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectKeyDisgust, 20101, Common::Rect(3529, 151, 3600, 296), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObjectKeyDisgust, 20101, Common::Rect(0, 151, 52, 296), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObjectKeyDisgust);
	_app->objectPresentationAddImageToRotation(kObjectKeyDisgust, 0, 20101, 0);
	_app->objectAddPresentation(kObjectKeyDisgust);
	_app->objectPresentationAddAnimationToRotation(kObjectKeyDisgust, 1, 20101, 1, 34, 12.5f, 4);
	_app->objectPresentationSetAnimationOnRotation(kObjectKeyDisgust, 1, 0, 20003);
	_app->objectPresentationShow(kObjectKeyDisgust, 0);
	_app->objectPresentationShow(kObjectKeyDisgust, 1);
	_app->objectAdd(kObjectRedfish, "GoldFish", "RH_GoldFish", 9);
	_app->objectAddBagAnimation(kObjectRedfish, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectRedfish, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectRedfish, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObjectRedfish, 20201, Common::Rect(792, 141, 1140, 291), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObjectRedfish);
	_app->objectPresentationAddAnimationToRotation(kObjectRedfish, 0, 20201, 0, 30, 12.5f, 4);
	_app->objectPresentationShow(kObjectRedfish, 0);
	_app->objectAdd(kObjectDolphin, "", "", 1);
	_app->objectAddPuzzleAccessibility(kObjectDolphin, kPuzzle20202, Common::Rect(374, 291, 480, 429), true,  kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectDolphin, kPuzzle20202, Common::Rect(374, 291, 480, 429), false, kCursorHotspot, 0);
	_app->objectAddPuzzleAccessibility(kObjectDolphin, kPuzzle20202, Common::Rect(  0, 420, 640, 464), true,  kCursorBack, 9);
	_app->objectAddRotationAccessibility(kObjectDolphin, 20201, Common::Rect(3524,  108, 3600, 247), false, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObjectDolphin, 20201, Common::Rect(   0,  108,   50, 247), false, kCursorHotspot, 2);
	_app->objectAddRotationAccessibility(kObjectDolphin, 20201, Common::Rect(3104, -322, 3600, 485), true,  kCursorMove, 1);
	_app->objectAddPresentation(kObjectDolphin);
	_app->objectPresentationAddAnimationToPuzzle(kObjectDolphin, 0, kPuzzle20202, "RHS02N01P02S01", kImageTypeBMP, Common::Point(175, 25), kDrawType1, 1000, 13, 12.5f, 4);
	_app->objectAddPresentation(kObjectDolphin);
	_app->objectPresentationAddAnimationToPuzzle(kObjectDolphin, 1, kPuzzle20202, "RHS02N01P02S04", kImageTypeBMP, Common::Point(173, 21), kDrawType1, 1000, 13, 12.5f, 4);
	_app->objectAddPresentation(kObjectDolphin);
	_app->objectPresentationAddAnimationToPuzzle(kObjectDolphin, 2, kPuzzle20202, "RHS02N01P02S02", kImageTypeBMP, Common::Point(178, 23), kDrawType1, 1000, 13, 12.5f, 4);
	_app->objectAddPresentation(kObjectDolphin);
	_app->objectPresentationAddAnimationToPuzzle(kObjectDolphin, 3, kPuzzle20203, "RHS02N01P02S03SD", kImageTypeBMP, Common::Point(274, 94), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObjectMedallion, "Necklace", "RH_Medallion", 1);
	_app->objectAddBagAnimation(kObjectMedallion, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectMedallion, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectMedallion, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAdd(kObject20204, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20204, 20202, Common::Rect(2343, 326, 2487, 408), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject20204);
	_app->objectPresentationAddImageToRotation(kObject20204, 0, 20202, 0);
	_app->objectAddPresentation(kObject20204);
	_app->objectPresentationAddAnimationToPuzzle(kObject20204, 1, kPuzzle20204, "RHS02N02P01S01", kImageTypeBMP, Common::Point(226, 186), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20301, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20301, 20301, Common::Rect(377, -139, 611, 108), true, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject20301, 20301, Common::Rect(641, 368, 1121, 551), true, kCursorHotspot, 1);
	_app->objectAddPuzzleAccessibility(kObject20301, kPuzzle20301, Common::Rect(240, 247, 386, 426), true, kCursorHotspot, 2);
	_app->objectAddPuzzleAccessibility(kObject20301, kPuzzle20301, Common::Rect(  0, 420, 640, 464), true, kCursorBack, 9);
	_app->objectAddPresentation(kObject20301);
	_app->objectPresentationAddAnimationToRotation(kObject20301, 0, 20301, 0, 20, 12.5f, 4);
	_app->objectPresentationAddAnimationToRotation(kObject20301, 0, 20301, 1, 20, 12.5f, 4);
	_app->objectPresentationShow(kObject20301, 0);
	_app->objectAddPresentation(kObject20301);
	_app->objectPresentationAddAnimationToPuzzle(kObject20301, 1, kPuzzle20301, "RHS03N01P01S01SD", kImageTypeBMP, Common::Point(280, 246), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20302, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20302, 20302, Common::Rect(170, -169, 558, 193), true, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject20302, 20302, Common::Rect(3471, 343, 3600, 566), true, kCursorHotspot, 1);
	_app->objectAddRotationAccessibility(kObject20302, 20302, Common::Rect(0, 343, 298, 566), true, kCursorHotspot, 1);
	_app->objectAddPresentation(kObject20302);
	_app->objectAddPresentation(kObject20302);
	_app->objectPresentationAddAnimationToPuzzle(kObject20302, 1, kPuzzle20302, "RHS03N02P01S01SD", kImageTypeBMP, Common::Point(294, 243), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20303, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20303, 20303, Common::Rect(511, -235, 798, 298), false, kCursorMove, 0);
	_app->objectAddRotationAccessibility(kObject20303, 20303, Common::Rect(1694, 350, 2116, 566), true, kCursorMove, 1);
	_app->objectAddPuzzleAccessibility(kObject20303, kPuzzle20303, Common::Rect(243, 161, 376, 442), true, kCursorHotspot, 2);
	_app->objectAddPresentation(kObject20303);
	_app->objectAddPresentation(kObject20303);
	_app->objectPresentationAddAnimationToPuzzle(kObject20303, 1, kPuzzle20303, "RHS03N03P01S01SD", kImageTypeBMP, Common::Point(230, 124), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20304, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20304, 20304, Common::Rect(2343, 326, 2487, 408), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject20304);
	_app->objectPresentationAddImageToRotation(kObject20304, 0, 20304, 0);
	_app->objectAddPresentation(kObject20304);
	_app->objectPresentationAddAnimationToPuzzle(kObject20304, 1, kPuzzle20204, "RHS02N02P01S01", kImageTypeBMP, Common::Point(226, 186), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20401, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20401, 20401, Common::Rect(1114, -160, 1564, 390), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject20401);
	_app->objectPresentationAddAnimationToRotation(kObject20401, 0, 20401, 0, 28, 12.5f, 4);
	_app->objectPresentationShow(kObject20401, 0);
	_app->objectPresentationPauseAnimation(kObject20401, 0);
	_app->objectAdd(kObject20402, "", "", 1);
	_app->objectAddPresentation(kObject20402);
	_app->objectPresentationAddAnimationToPuzzle(kObject20402, 0, kPuzzle20401, "RHS04N01P01S01SD", kImageTypeBMP, Common::Point(277, 124), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject20402);
	_app->objectPresentationAddAnimationToPuzzle(kObject20402, 1, kPuzzle20401, "RHS04N01P01S02", kImageTypeBMP, Common::Point(216, 116), kDrawType1, 1000, 25, 12.5f, 4);
	_app->objectPresentationShow(kObject20402, 1);
	_app->objectAdd(kObjectAntiGCells2, "Anti gravitation cells", "RH_AntiG", 1);
	_app->objectAddBagAnimation(kObjectAntiGCells2, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObjectAntiGCells2, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObjectAntiGCells2, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddPuzzleAccessibility(kObjectAntiGCells2, kPuzzle20401, Common::Rect(250, 121, 345, 346), true, kCursorHotspot, 0);
	_app->objectAdd(kObject20404, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20404, 20402, Common::Rect(2315, 207, 2409, 272), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject20404);
	_app->objectPresentationAddImageToRotation(kObject20404, 0, 20402, 0);
	_app->objectAddPresentation(kObject20404);
	_app->objectPresentationAddAnimationToPuzzle(kObject20404, 1, kPuzzle20204, "RHS02N02P01S01", kImageTypeBMP, Common::Point(226, 186), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20501, "Daughter of the Rhine", "", 1);
	_app->objectAddRotationAccessibility(kObject20501, 20501, Common::Rect(270, 317, 592, 516), true, kCursorHotspot, 0);
	_app->objectAddRotationAccessibility(kObject20501, 20501, Common::Rect(341, -122, 553, 200), true, kCursorMove, 1);
	_app->objectAddRotationAccessibility(kObject20501, 20501, Common::Rect(2053, -148, 2429, 324), false, kCursorMove, 2);
	_app->objectAddPuzzleAccessibility(kObject20501, kPuzzle20502, Common::Rect(226, 184, 462, 458), false, kCursorHotspot, 3);
	_app->objectAddPuzzleAccessibility(kObject20501, kPuzzle20502, Common::Rect(  0, 420, 640, 464), true,  kCursorBack, 9);
	_app->objectAddPresentation(kObject20501);
	_app->objectPresentationAddImageToRotation(kObject20501, 0, 20501, 0);
	_app->objectPresentationShow(kObject20501, 0);
	_app->objectAddPresentation(kObject20501);
	_app->objectPresentationAddAnimationToPuzzle(kObject20501, 1, kPuzzle20501, "RHS05N01P01S01SF", kImageTypeBMP, Common::Point(197, 116), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject20501);
	_app->objectPresentationAddAnimationToPuzzle(kObject20501, 2, kPuzzle20502, "RHS05N01P02S01SD", kImageTypeBMP, Common::Point(247, 161), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddPresentation(kObject20501);
	_app->objectPresentationAddAnimationToPuzzle(kObject20501, 3, kPuzzle20503, "RHS05N02P01S01", kImageTypeBMP, Common::Point(184, 226), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAdd(kObject20502, "", "", 1);
	_app->objectAddRotationAccessibility(kObject20502, 20503, Common::Rect(2343, 326, 2487, 408), true, kCursorHotspot, 0);
	_app->objectAddPresentation(kObject20502);
	_app->objectPresentationAddImageToRotation(kObject20502, 0, 20503, 0);
	_app->objectAddPresentation(kObject20502);
	_app->objectPresentationAddAnimationToPuzzle(kObject20502, 1, kPuzzle20204, "RHS02N02P01S01", kImageTypeBMP, Common::Point(226, 186), kDrawType1, 1000, 20, 12.5f, 4);
	_app->objectAddRotationAccessibility(kObjectDivingHelmet2, 20601, Common::Rect(337, -75, 557, 195), true, kCursorMove, 0);
	_app->objectAdd(kObject20700, "Rhine Gold", "RH_Gold", 1);
	_app->objectAddBagAnimation(kObject20700, kImageTypeTGA, kDrawType3, 20, 12.5f, 4);
	_app->objectSetActiveCursor(kObject20700, Common::Point(22, 22), 20, kCursorTypeAnimated, 12.5f, 4, kLoadFromListIcon);
	_app->objectSetPassiveCursor(kObject20700, Common::Point(22, 22), 0, kCursorTypeImage, 0, 0, kLoadFromCursor);
	_app->objectAddRotationAccessibility(kObject20700, 20701, Common::Rect(381, 61, 530, 231), true, kCursorHotspot, 0);
	_app->soundAdd(23005, kSoundTypeAmbientMusic, "1746.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAddAmbientSound(20010, 23005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20020, 23005, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20030, 23005, 100, 0, 1, 1, 10);
	_app->soundAdd(23013, kSoundTypeAmbientMusic, "1747.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->puzzleAddAmbientSound(kPuzzle20000, 23013, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20011, 23013, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20021, 23013, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20031, 23013, 100, 0, 1, 1, 10);
	_app->soundAdd(23004, kSoundTypeAmbientMusic, "1748.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAddAmbientSound(20101, 23004, 90, 0, 1, 1, 10);
	_app->soundAdd(23002, kSoundTypeAmbientMusic, "1749.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAddAmbientSound(20201, 23002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20202, 23002, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20203, 23002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20202, 23002, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20203, 23002, 100, 0, 1, 1, 10);
	_app->soundAdd(23007, kSoundTypeAmbientMusic, "1750.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAddAmbientSound(20301, 23007, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20302, 23007, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20303, 23007, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20304, 23007, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20305, 23007, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20301, 23007, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20302, 23007, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20303, 23007, 100, 0, 1, 1, 10);
	_app->soundAdd(23003, kSoundTypeAmbientMusic, "1751.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAddAmbientSound(20401, 23003, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20402, 23003, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20403, 23003, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20401, 23003, 100, 0, 1, 1, 10);
	_app->soundAdd(23006, kSoundTypeAmbientMusic, "1752.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAddAmbientSound(20501, 23006, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20503, 23006, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20504, 23006, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20501, 23006, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20502, 23006, 100, 0, 1, 1, 10);
	_app->puzzleAddAmbientSound(kPuzzle20503, 23006, 100, 0, 1, 1, 10);
	_app->soundAdd(23001, kSoundTypeAmbientMusic, "1753.was", _app->getConfiguration().ambientMusic.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAddAmbientSound(20601, 23001, 100, 0, 1, 1, 10);
	_app->rotationAddAmbientSound(20701, 23001, 100, 0, 1, 1, 10);
	_app->soundAdd(23014, kSoundTypeAmbientEffect, "1754.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->rotationAdd3DSound(20101, 23014, 1, 1, 20, 100, 228.0f, 15);
	_app->rotationSet3DSoundOff(20101, 23014);
	_app->soundAdd(23009, kSoundTypeAmbientEffect, "1755.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(23010, kSoundTypeAmbientEffect, "1756.was", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(23011, kSoundTypeAmbientEffect, "1757.wav", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(23012, kSoundTypeAmbientEffect, "1758.wac", _app->getConfiguration().ambientEffect.loadFrom, 2, _app->getConfiguration().backgroundMusic.soundChunck);
	_app->soundAdd(22001, kSoundTypeDialog, "1626.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(22002, kSoundTypeDialog, "1627.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(22003, kSoundTypeDialog, "1628.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20011, kSoundTypeDialog, "1759.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20021, kSoundTypeDialog, "1760.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20031, kSoundTypeDialog, "1761.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20012, kSoundTypeDialog, "1762.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20022, kSoundTypeDialog, "1763.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20032, kSoundTypeDialog, "1764.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20013, kSoundTypeDialog, "1765.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20023, kSoundTypeDialog, "1766.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20033, kSoundTypeDialog, "1767.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20201, kSoundTypeDialog, "1768.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20202, kSoundTypeDialog, "1769.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20301, kSoundTypeDialog, "1770.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20302, kSoundTypeDialog, "1771.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20303, kSoundTypeDialog, "1772.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20304, kSoundTypeDialog, "1773.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20401, kSoundTypeDialog, "1774.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20402, kSoundTypeDialog, "1775.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20501, kSoundTypeDialog, "1776.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20502, kSoundTypeDialog, "1777.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20503, kSoundTypeDialog, "1778.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->soundAdd(20504, kSoundTypeDialog, "1779.wac", _app->getConfiguration().dialog.loadFrom, 2, _app->getConfiguration().dialog.soundChunck);
	_app->varDefineByte(21000, 0);
	_app->varDefineByte(21001, 0);
	_app->varDefineByte(20200, 1);
	_app->varDefineByte(20201, 1);
	_app->varDefineByte(20202, 0);
	_app->varDefineByte(20301, 0);
	_app->varDefineByte(20500, 0);
	_app->varDefineByte(20009, 0);
	_app->varDefineWord(20000, 20000);
}

void ZoneRHRing::onSetup(SetupType type) {
	switch (type) {
	default:
		break;

	case kSetupTypeNone:
		_app->bagRemove(kObjectAntiGCells);
		_app->soundPlay(23005, kSoundLoop);
		_app->playMovie("1706");
		_app->puzzleSetActive(kPuzzle22001);
		_app->soundPlay(22001);
		break;

	case kSetupType10:
		_app->onSetupLoadTimers("alb", 90017, 90021, 90025);
		break;

	case kSetupType999:
		_app->bagRemoveAll();
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->bagAdd(kObjectMedallion);
		_app->bagAdd(kObjectKeyIndifference);
		_app->bagAdd(kObjectKeyMistrust);
		_app->bagAdd(kObjectKeySelfishness);
		_app->bagAdd(kObjectKeyDisgust);
		_app->rotationSetActive(20501);
		break;
	}
}

void ZoneRHRing::onSwitch(uint32 type) {
	if (type == 0)
		_app->setupZone(kZoneRH, kSetupTypeNone);
}

void ZoneRHRing::onTimer(TimerId id) {
	if (id != kTimer0)
		return;

	// FIXME: Since we are called synchronously, is this needed? If so, replace by mutex?
	if (_disableTimerRH)
		return;

	_disableTimerRH = true;

	float alp = _app->rotationGetAlp(20401) - 35.0f;
	if (alp > 0.0f && alp < 146.0f) {
		uint32 frame = (uint32)(alp * 0.2631578947368421f);

		if (frame >= 1 && frame <= 28 && _frameNumberTimerRH != frame) {
			_app->objectPresentationSetAnimationActiveFrame(20401, 0, frame);
			_frameNumberTimerRH = frame;
		}
	}

	_disableTimerRH = false;
}

void ZoneRHRing::onButtonUp(ObjectId id, Id target, Id, uint32, const Common::Point &) {
	switch (id) {
	default:
		break;

	case kObjectDivingHelmet2:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
				_app->playMovie("1668");
				_app->soundPlay(23010, kSoundLoop);
				_app->rotationSetAlp(20701, 0.0);
				_app->rotationSetActive(20701);
			}

			_app->cursorDelete();
		} else {
			_app->playMovie("1669");
			_app->exitToMenu(kMenuAction3);
		}
		break;

	case kObject20000:
	case kObject20001:
	case kObject20002:
	case kObject20003:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (target) {
			_app->puzzleSetActive(kPuzzle20000);
			_app->objectPresentationShow(kObject21001, 0);
			_app->soundPlay(_app->varGetByte(21001) + 20031);
		} else {
			_app->puzzleSetActive(kPuzzle20000);
			_app->objectPresentationShow(kObject21001, 0);
			_app->soundPlay(_app->varGetByte(21001) + 20021);
		}
		break;

	case kObjectKeyIndifference:
	case kObjectKeyMistrust:
	case kObjectKeySelfishness:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		_app->bagAdd(id);
		_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
		_app->objectPresentationHide(id);
		_app->objectSetAccessibilityOff(id);
		_app->rotationSetMovabilityOff(10 * (_app->varGetByte(21001) + 2001), 0, 0);
		_app->rotationSetMovabilityOn(10 * (_app->varGetByte(21001) + 2001), 1, 1);
		_app->rotationSetActive(10 * (_app->varGetByte(21001) + 2001));
		if (_app->bagHas(kObjectKeyIndifference)
		 && _app->bagHas(kObjectKeyMistrust)
		 && _app->bagHas(kObjectKeySelfishness)
		 && _app->bagHas(kObjectKeyDisgust)) {
			_app->rotationSetMovabilityOn(20101, 1, 1);
			_app->soundPlay(23009);
		}
		break;

	case kObjectKeyDisgust:
		if (!_app->bagHasClickedObject()) {
			if (!target) {
				_app->bagAdd(kObjectKeyDisgust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->objectPresentationHide(kObjectKeyDisgust, 0);
				_app->objectSetAccessibilityOff(kObjectKeyDisgust, 0, 1);

				if (_app->bagHas(kObjectKeyIndifference)
				 && _app->bagHas(kObjectKeyMistrust)
				 && _app->bagHas(kObjectKeySelfishness)
				 && _app->bagHas(kObjectKeyDisgust)) {
					_app->rotationSetMovabilityOn(20101, 1, 1);
					_app->soundPlay(23009);
				}
			}
			break;
		}

		_app->cursorDelete();
		_app->setField74(false);
		break;

	case kObjectRedfish:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectRedfish) {
				_app->bagRemove(kObjectRedfish);
				_app->varSetByte(20200, 1);
				_app->objectSetAccessibilityOn(kObjectDolphin, 5, 5);
				_app->objectSetAccessibilityOff(kObjectDolphin, 3, 4);
				_app->objectPresentationShow(kObjectRedfish, 0);
			}

			_app->setField74(false);
			_app->cursorDelete();
			break;
		}

		if (target == 1 && _app->varGetByte(20200) == 1) {
			if (_app->varGetByte(20202)) {
				_app->setField74(false);
				_app->playMovie("1694");
			} else {
				_app->playMovie("1693");
				_app->bagAdd(kObjectRedfish);
				_app->varSetByte(20200, 0);
				_app->objectSetAccessibilityOff(kObjectDolphin, 5, 5);
				_app->objectSetAccessibilityOn(kObjectDolphin, 3, 4);
				_app->objectPresentationHide(kObjectRedfish, 0);
			}
		}
		break;

	case kObjectDolphin:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 0:
				if (_app->bagGetClickedObject() == kObjectMedallion) {
					_app->varSetByte(20202, 0);
					_app->bagRemove(kObjectMedallion);
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 0);
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectRedfish) {
					_app->bagRemove(kObjectRedfish);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
					_app->objectSetAccessibilityOff(kObjectRedfish);
					_app->varSetByte(20202, 2);
					_app->playMovie("1688");
					_app->objectPresentationHide(kObjectDolphin);
					_app->puzzleSetActive(kPuzzle20203);
					_app->soundPlay(20201);
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20201) == 1) {
				if (!_app->varGetByte(20202)) {
					_app->varSetByte(20202, 1);
					_app->bagAdd(kObjectMedallion);
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 1);
				}
			}
			break;

		case 1:
			if (_app->varGetByte(20201)) {
				if (_app->varGetByte(20202)) {
					_app->playMovie("1690");
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 1);
				} else {
					_app->playMovie("1689");
					_app->objectPresentationHide(kObjectDolphin);
					_app->objectPresentationShow(kObjectDolphin, 0);
				}
			} else {
				_app->varGetByte(20202);
			}

			_app->puzzleSetActive(kPuzzle20202);
			break;

		case 9:
			_app->playMovie(_app->varGetByte(20202) ? "1692" : "1691");
			_app->rotationSetActive(20201);
			break;
		}
		break;

	case kObject20204:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyDisgust) {
				_app->bagRemove(kObjectKeyDisgust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->objectSetAccessibilityOff(kObject20204);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20202);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20301:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectMedallion) {
				if (target == 2) {
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->bagRemove(kObjectMedallion);
					_app->playMovie("1676");
					_app->objectSetAccessibilityOff(kObject20301, 1, 1);
					_app->rotationSetActive(20301);
				}
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 1:
			_app->rotationSetRolTo(20301, 42.0f, 23.0f, 85.7f);
			_app->playMovie("1679");
			_app->puzzleSetActive(kPuzzle20301);
			_app->soundPlay(20301);
			break;

		case 9:
			_app->playMovie("1680");
			_app->rotationSetActive(20301);
			break;
		}

		if (_app->varGetByte(20202) == 2) {
			if (!_app->bagHas(kObjectMedallion)) {
				_app->playMovie("1678");
				_app->rotationSetAlp(20302, 10.0);
				_app->rotationSetActive(20302);
				break;
			}
		}

		_app->playMovie("1677");
		_app->exitToMenu(kMenuAction1);
		break;

	case kObject20302:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->bagHas(kObjectMedallion)) {
				_app->playMovie("1682");
				_app->rotationSetAlp(20303, 10.0);
				_app->rotationSetActive(20303);
				break;
			}

			_app->playMovie("1681");
			_app->exitToMenu(kMenuAction1);
			break;

		case 1:
			_app->rotationSetRolTo(20302, 320.0f, 26.0f, 85.7f);
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
			_app->objectSetAccessibilityOff(kObject20302, 1, 2);
			_app->playMovie("1683", 0.0);
			_app->puzzleSetActive(kPuzzle20302);
			_app->soundPlay(20302);
			break;
		}
		break;

	case kObject20303:
		if (_app->bagHasClickedObject()) {
			switch (target) {
			default:
				break;

			case 1:
				if (_app->bagGetClickedObject() == kObjectDivingHelmet2) {
					_app->objectPresentationShow(kObject20303, 0);
					_app->playMovie("1684");
					_app->soundPlay(23010, kSoundLoop);
					_app->puzzleSetActive(kPuzzle20303);
				}
				break;

			case 2:
				if (_app->bagGetClickedObject() == kObjectMedallion) {
					_app->varSetByte(20301, 1);
					_app->objectSetAccessibilityOn(kObject20303, 0, 0);
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->objectSetAccessibilityOff(kObject20303, 1, 2);
					_app->objectPresentationHide(kObject20303, 0);
					_app->soundPlay(20303);
				}
				break;
			}

			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20301) == 1) {
				_app->playMovie("1685");
				_app->rotationSetActive(20304);
			}
			break;

		case 1:
			_app->rotationSetRolTo(20303, 140.0f, 26.0f, 85.7f);
			_app->playMovie("1686");
			_app->exitToMenu(kMenuAction3);
			break;

		case 9:
			_app->soundStop(23010, 1024);
			_app->playMovie("1687");
			_app->rotationSetActive(20303);
			break;
		}
		break;

	case kObject20304:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyMistrust) {
				_app->bagRemove(kObjectKeyMistrust);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->objectSetAccessibilityOff(kObject20304);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20304);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20401:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectBrutality) {
				_app->playMovie("1675");
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 3.0f);
				_app->puzzleSetActive(kPuzzle20401);
			}

			_app->cursorDelete();
		}
		break;

	case kObjectAntiGCells2:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectMedallion) {
				_app->bagRemove(kObjectMedallion);
				_app->objectPresentationHide(kObject20402, 1);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->soundPlay(20401);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20404:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeySelfishness) {
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->bagRemove(kObjectKeySelfishness);
				_app->objectSetAccessibilityOff(kObject20404);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20402);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20501:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		switch (target) {
		default:
			break;

		case 0:
			if (_app->varGetByte(20500)) {
				_app->puzzleSetActive(kPuzzle20502);
			} else {
				_app->varSetByte(20500, 1);
				_app->soundStop(23011, 1024);
				_app->puzzleSetActive(kPuzzle20501);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
				_app->soundPlay(20501);
			}
			break;

		case 1:
			if (_app->varGetByte(20500) >= 2) {
				if (_app->varGetByte(20500) == 3) {
					_app->playMovie("1671");
					_app->varSetFloat(90005, _app->varGetFloat(90005) + 2.0f);
					_app->rotationSetAlp(20503, 0.0);
					_app->rotationSetActive(20503);
				}
			} else {
				_app->playMovie("1670");
				_app->exitToMenu(kMenuAction4);
			}
			break;

		case 2:
			if (_app->varGetByte(20500) == 2) {
				_app->varSetByte(20500, 3);
				_app->soundStop(23011, 1024);
				_app->playMovie("1672");
				_app->objectSetAccessibilityOn(kObject20501, 1, 1);
				_app->objectSetAccessibilityOff(kObject20501, 2, 2);
				_app->puzzleSetActive(kPuzzle20503);
				_app->soundPlay(20503);
			}
			break;

		case 3:
			_app->playMovie("1673");
			_app->varSetByte(20500, 2);
			_app->objectPresentationHide(kObject20501, 0);
			_app->objectSetAccessibilityOff(kObject20501, 0, 1);
			_app->objectSetAccessibilityOn(kObject20501, 2, 2);
			break;

		case 9:
			_app->rotationSetActive(20501);
			break;
		}
		break;

	case kObject20502:
		if (_app->bagHasClickedObject()) {
			if (_app->bagGetClickedObject() == kObjectKeyIndifference) {
				_app->bagRemove(kObjectKeyIndifference);
				_app->varSetFloat(90005, _app->varGetFloat(90005) + 5.0f);
				_app->varSetByte(20500, 4);
				_app->objectSetAccessibilityOff(kObject20502);
				_app->puzzleSetActive(kPuzzle20204);
				_app->soundPlay(20504);
			}

			_app->cursorDelete();
		}
		break;

	case kObject20700:
		if (_app->bagHasClickedObject()) {
			_app->cursorDelete();
			break;
		}

		if (_app->varGetByte(20500) == 4) {
			_app->varSetFloat(90005, _app->varGetFloat(90005) + 1.0f);
			_app->soundStopAll(1024);
			_app->playMovie("1666");
			_app->bagRemove(kObjectAntiGCells2);
			_app->onSwitchZone(kZoneNI, 3);
		}
		else {
			_app->playMovie("1667");
			_app->exitToMenu(kMenuAction2);
		}
		break;
	}
}

void ZoneRHRing::onAnimationNextFrame(Id animationId, const Common::String &, uint32 frame, uint32) {
	switch (animationId){
	default:
		break;

	case 20001:
		if (frame == 5) {
			_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 2);
			_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20001), 4);
			_app->soundPlay(_app->varGetByte(21001) + 20011);
		}
		break;

	case 20003:
		if (frame == 2)
			_app->soundPlay(23014);
		break;
	}
}

void ZoneRHRing::onSound(Id id, SoundType, uint32, bool process) {
	if (!process)
		return;

	switch (id) {
	default:
		break;

	case 20011:
	case 20012:
	case 20013:
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 2);
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001), 4);
		_app->objectPresentationSetAnimationOnPuzzle((ObjectId)(_app->varGetByte(21001) + 20001), 2, 0, 20002);
		_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20001), 3);
		_app->objectSetAccessibilityOn((ObjectId)(_app->varGetByte(21001) + 20001), 0, 1);
		break;

	case 20021:
	case 20022:
	case 20023:
		_app->playMovie(Common::String::format("rh_%d", _app->varGetByte(21001) + 1));
		_app->playMovie("1696");
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->rotationSetMovabilityOff(10 * (_app->varGetByte(21001) + 2001), 0, 0);
		_app->rotationSetMovabilityOn(10 * (_app->varGetByte(21001) + 2001), 1, 1);
		_app->rotationSetActive(10 * (_app->varGetByte(21001) + 2001));
		break;

	case 20031:
	case 20032:
	case 20033:
		_app->playMovie(Common::String::format("rh_%d_l0", _app->varGetByte(21001) + 1));
		_app->objectSetAccessibilityOff((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->objectSetAccessibilityOn((ObjectId)(_app->varGetByte(21001) + 20004));
		_app->objectPresentationShow((ObjectId)(_app->varGetByte(21001) + 20004), 0);
		_app->objectPresentationHide((ObjectId)(_app->varGetByte(21001) + 20001));
		_app->puzzleSetActive((PuzzleId)(10 * _app->varGetByte(21001) + 20011));
		break;

	case 20201:
		_app->playMovie("1698");
		_app->bagAdd(kObjectMedallion);
		_app->soundPlay(23009);
		_app->objectSetAccessibilityOff(kObjectDolphin);
		_app->rotationSetActive(20201);
		_app->rotationSetMovabilityOn(20201, 0, 0);
		break;

	case 20202:
		_app->playMovie("1699");
		_app->objectPresentationShow(kObject21003, 0);
		_app->objectPresentationShow(kObject20204, 0);
		_app->rotationSetMovabilityOff(20202, 2, 2);
		_app->rotationSetMovabilityOff(20202, 0, 0);
		_app->rotationSetMovabilityOff(20203, 0, 0);
		_app->rotationSetMovabilityOn(20202, 1, 1);
		_app->rotationSetMovabilityOn(20203, 1, 3);
		_app->rotationSetActive(20202);
		break;

	case 20301:
		_app->objectPresentationHide(kObject20301, 1);
		break;

	case 20302:
		_app->objectPresentationHide(kObject20302, 1);
		_app->bagAdd(kObjectMedallion);
		_app->playMovie("1700");
		_app->rotationSetBet(20302, 0.3f);
		_app->rotationSetActive(20302);
		break;

	case 20303:
		_app->soundStop(23010, 1024);
		_app->playMovie("1687");
		_app->soundPlay(23009);
		_app->rotationSetAlp(20303, 325.0);
		_app->rotationSetBet(20303, 0.3f);
		_app->rotationSetActive(20303);
		break;

	case 20304:
		_app->playMovie("1701");
		_app->objectPresentationShow(kObject20304, 0);
		_app->objectPresentationShow(kObject21003, 1);
		_app->rotationSetMovabilityOff(20304, 2, 2);
		_app->rotationSetMovabilityOff(20304, 0, 0);
		_app->rotationSetMovabilityOff(20305, 0, 0);
		_app->rotationSetMovabilityOn(20304, 1, 1);
		_app->rotationSetMovabilityOn(20305, 1, 2);
		_app->rotationSetActive(20304);
		break;

	case 20401:
		_app->playMovie("1674");
		_app->bagAdd(kObjectAntiGCells2);
		_app->soundPlay(23009);
		_app->objectPresentationHide(kObject20402);
		_app->playMovie("1702");
		_app->rotationSetAlp(20402, 0.0);
		_app->rotationSetActive(20402);
		break;

	case 20402:
		_app->playMovie("1703");
		_app->objectPresentationShow(kObject20404, 0);
		_app->objectPresentationShow(kObject21003, 2);
		_app->rotationSetMovabilityOff(20402, 0, 0);
		_app->rotationSetMovabilityOff(20403, 0, 0);
		_app->rotationSetMovabilityOn(20402, 1, 1);
		_app->rotationSetMovabilityOn(20403, 1, 2);
		_app->rotationSetActive(20402);
		break;

	case 20504:
		_app->playMovie("1705");
		_app->objectPresentationShow(kObject20502, 0);
		_app->rotationSetMovabilityOff(20503, 0, 0);
		_app->rotationSetMovabilityOff(20504, 0, 0);
		_app->rotationSetMovabilityOn(20504, 1, 2);
		_app->rotationSetActive(20503);
		break;

	case 20501:
		_app->puzzleSetActive(kPuzzle20502);
		_app->soundPlay(20502);
		break;

	case 20502:
		_app->objectPresentationHide(kObject20501, 2);
		_app->objectSetAccessibilityOn(kObject20501, 3, 4);
		break;

	case 20503:
		_app->objectPresentationHide(kObject20501, 3);
		_app->playMovie("1704");
		_app->rotationSetAlp(20501, 0.0);
		_app->rotationSetActive(20501);
		break;

	case 22001:
		_app->puzzleSetActive(kPuzzle22003);
		_app->soundPlay(22003);
		break;

	case 22002:
		_app->playMovie("1697");
		_app->bagAdd(kObjectBrutality);
		_app->bagAdd(kObjectDivingHelmet2);
		_app->rotationSetAlp(20010, 1.5);
		_app->rotationSetBet(20010, -4.3f);
		_app->rotationSetRan(20010, 79.3f);
		_app->rotationSetActive(20010);
		break;

	case 22003:
		_app->puzzleSetActive(kPuzzle22002);
		_app->soundPlay(22002);
		break;
	}
}

void ZoneRHRing::onBeforeRide(Id movabilityFrom, Id, uint32, Id, MovabilityType) {
	if (movabilityFrom == 20401)
		_app->timerStop(kTimer0);
}

void ZoneRHRing::onAfterRide(Id movabilityFrom, Id, uint32, Id, MovabilityType movabilityType) {
	if (movabilityFrom == 20401)
		_app->timerStart(kTimer0, 50);

	switch (movabilityType) {
	default:
		break;

	case kMovabilityRotationToRotation:
		if (movabilityFrom >= 20010) {
			_app->varSetByte(21001, 0);

			if (_app->bagHas(kObjectKeyIndifference)) {
				_app->rotationSetMovabilityOn(20010, 0, 0);
				_app->rotationSetMovabilityOff(20010, 1, 1);
			}
		}

		if (movabilityFrom >= 20020) {
			_app->varSetByte(21001, 1);

			if (_app->bagHas(kObjectKeyMistrust)) {
				_app->rotationSetMovabilityOn(20020, 0, 0);
				_app->rotationSetMovabilityOff(20020, 1, 1);
			}
		}

		if (movabilityFrom >= 20030) {
			_app->varSetByte(21001, 2);

			if (_app->bagHas(kObjectKeySelfishness)) {
				_app->rotationSetMovabilityOn(20030, 0, 0);
				_app->rotationSetMovabilityOff(20030, 1, 1);
			}
		}
		break;

	case kMovabilityRotationToPuzzle:
		if (movabilityFrom >= 20011 && movabilityFrom <= 20031) {
			ObjectId id = 20001 + _app->varGetByte(21001);
			_app->objectPresentationShow(id, 0);
			_app->objectPresentationSetAnimationOnPuzzle(id, 2, 0, 20001);
			_app->objectPresentationShow(id, 2);
		}
		break;
	}
}

} // End of namespace Ring
