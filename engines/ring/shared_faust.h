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
 *
 * $URL$
 * $Id$
 *
 */

#ifndef RING_SHARED_FAUST_H
#define RING_SHARED_FAUST_H

namespace FaustGame {

//////////////////////////////////////////////////////////////////////////
// Ring Enumerations
//////////////////////////////////////////////////////////////////////////
enum PuzzleId {
	kPuzzleGeneralMenu = 90000,
};

enum ObjectId {
	kObject1                             = 1,
	kObjectPipe                          = 21,
	kObjectCodingWheel                   = 24,
	kObjectAshShovel                     = 12001,
	kObjectStrainer                      = 12002,
	kObjectBronzeKey                     = 12008,
	kObjectBurnedKey                     = 12014,
	kObjectLotteryTicket                 = 12029,
	kObjectButcherKnife                  = 12033,
	kObjectTooth                         = 12035,
	kObjectShovelWithAshes               = 12037,
	kObjectRing                          = 13119,
	kObjectStChristopherMedallion        = 13120,
	kObjectWoodLog                       = 13122,
	kObjectBucketWithMolasse             = 13125,
	kObjectBucket                        = 13126,
	kObjectOldNew                        = 13128,
	kObjectMatches                       = 13129,
	kObjectBottle                        = 13131,
	kObjectBottleOfWhiskey               = 13132,
	kObjectBottleOfMolasse               = 13133,
	kObjectTap                           = 13134,
	kObjectBaseballBat                   = 13135,
	kObjectLotteryTicket2                = 13899,
	kObjectRat                           = 17021,
	kObjectKey                           = 17101,
	kObjectCylinder                      = 17202,
	kObjectWhip                          = 17301,
	kObjectReel                          = 17302,
	kObjectHandle                        = 17303,
	kObjectRolodex                       = 31004,
	kObjectBathtubKey                    = 52222,
	kObjectKey2                          = 53112,
	kObjectMatches2                      = 53125,
	kObjectPiecesOfPicture               = 54203,
	kObjectPostcards                     = 55211,
	kObjectLoveIsland                    = 55212,
	kObjectGreatInventor                 = 55213,
	kObjectVisitUs                       = 55214,
	kObjectEnchantedIsland               = 55215,
	kObjectGreatAttractions              = 55216,
	kObjectBootlegging                   = 55217,
	kObjectBathOfGiselle                 = 55218,
	kObjectPencil                        = 56003,
	kObjectNecklace                      = 61202,
	kObjectWhip2                         = 61900,
	kObjectRing2                         = 71001,
	kObjectFuse                          = 71004,
	kObjectFrankFirstPrize               = 81207,
	kObjectThinPencil                    = 81209,
	kObjectCompassNeedle                 = 81210,
	kObjectBoxOfSlides                   = 81212,
	kObjectMagnifyingGlass               = 81213,
	kObjectLittleWig                     = 81218,
	kObjectKey3                          = 81219,
	kObjectKey4                          = 81304,
	kObjectYellowBulb                    = 81402,
	kObjectRedBulb                       = 81403,
	kObjectDiaryOfASeducer               = 81407,
	kObjectHomunculus                    = 97001,
	kObjectKey5                          = 112013,
	kObjectPaperKnife                    = 112020,
	kObjectSealingRing                   = 112021,
	kObjectMagnifyingGlass2              = 112025,
	kObjectWoodenHead                    = 112026,
	kObjectGeologicalSurveyMap           = 112029,
	kObjectTape1                         = 112030,
	kObjectMatches3                      = 112034,
	kObjectTape2                         = 112090,
	kObjectTape3                         = 112091,
	kObjectLead                          = 112601,
	kObjectMagnesium                     = 112602,
	kObjectMoonMilk                      = 112603,
	kObjectMercury                       = 112604,
	kObjectSunSpirit                     = 112605,
	kObjectRedWax                        = 112606,
	kObjectWhiteWax                      = 112607,
	kObjectBlackWax                      = 112608,
	kObjectVitriol                       = 112609,
	kObjectSulphur                       = 112610,
	kObjectRockSalt                      = 112611,
	kObjectTape4                         = 114501,
	kObjectShowel                        = 114504,
	kObjectHandle2                       = 114508,
	kObjectMonocular                     = 114511,
	kObjectHandle3                       = 114514,
	kObjectKey6                          = 114518,
	kObjectMirror                        = 114519,
	kObjectSaltOfTheWise                 = 114523,
	kObjectCommonSalt                    = 114524,
	kObjectNail                          = 120006,
	kObjectNecklace2                     = 120007,
	kObjectPeacockFeathers               = 120012,
	kObjectFlour                         = 120015,
	kObjectStrongLeatherStrap            = 120021,
	kObjectMask                          = 120023,
	kObjectShoeLaces                     = 120025,
	kObjectRespirationGrille             = 120026,
	kObjectGauze                         = 120030,
	kObjectSoftTissue                    = 120031,
	kObjectKalinkaJadeAmoulet            = 120032,
	kObjectLittleSilverMoon              = 120033,
	kObjectWeddingRing                   = 120035,
	kObjectMatches4                      = 120036,
	kObjectHardLeather                   = 120042,
	kObjectSoftLeather                   = 120043,
	kObjectPiecesOfSoftLeather           = 120044,
	kObjectPiecesOfSoftLeatherWithHoles  = 120045,
	kObjectSoftLeatherStrap              = 120046,
	kObjectTheFourHerbs                  = 120053,
	kObjectMixedHerbs                    = 120055,
	kObjectCompletedMask                 = 120057,
	kObjectCork                          = 120062,
	kObjectMovieReel                     = 142001,
	kObjectIHaveADreamMartinLutherKing   = 310003,
	kObjectTheodoreRooseveltTheNewDeal   = 310004,
	kObjectLostParadisesMilton           = 310005,
	kObjectHopeAndGloryBrittain1900_1980 = 310006,
	kObjectInMemoriaVirgile              = 310007,
	kObject1917TheYearAmericaWentToWar   = 310008,
	kObjectAndTheAngelsSang              = 310009,
	kObjectSignedWithTheirHonour         = 310010,
	kObjectAMidSummerNightDream          = 310011,
	kObjectContractAndRelatedObligation  = 310011,
	kObjectWithWilliamBurroughs          = 310013,
	kObjectMephistoKlausMann             = 310014,
	kObjectSpoon                         = 531001,
	kObjectApple                         = 531011,
	kObjectCola                          = 531012,
	kObjectMenthol                       = 531013,
	kObjectGrenadine                     = 531014,
	kObjectTeddyBear                     = 531021,
	kObjectFrozenPaper                   = 532122,
	kObjectChain                         = 532126,
	kObjectPiece1                        = 542101,
	kObjectPiece2                        = 542102,
	kObjectPiece3                        = 542103,
	kObjectPiece4                        = 542104,
	kObjectPiece5                        = 542105,
	kObjectPiece6                        = 542106
};

} // End of namespace FaustGame

#endif // RING_SHARED_FAUST_H
