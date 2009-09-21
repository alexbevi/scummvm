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

#include "gargoyle/frotz.h"
#include "gargoyle/frotz_interp.h"
#include "gargoyle/gargoyle.h"

#include "common/EventRecorder.h"
#include "common/events.h"
#include "common/md5.h"
#include "graphics/cursorman.h"
#include "engines/engine.h"

namespace Frotz {

struct FrotzGameEntry {
	const char *target;
	Gargoyle::GargoyleGameId gameId;
	Common::Language lang;
	const char *md5[15];
};

static const FrotzGameEntry frotz_games[] = {
	// Infocom games
	{"amfv", Gargoyle::GID_ZCODE_AMFV, Common::EN_ANY,
			{"b7ffaed0ca4a90450f92b34066133377",
			 "1e37dbcf7ccc9244dbfc3229796362f4",
			 NULL},
	},
	{"arthur", Gargoyle::GID_ZCODE_ARTHUR, Common::EN_ANY,
			{"ced2c66d03a49de0e8190b468332f081",
			 "13d13f375f85a874c82a8ac7ad69dc41",
			 NULL}
	},
	{"ballyhoo", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"7944e832a7d7b34037c7b6791de43dbd", NULL}
	},
	{"beyondzork", Gargoyle::GID_ZCODE_BEYOND_ZORK, Common::EN_ANY,
			{"a5547795def620d0a75a064f9a37ab2d",
			 "73948f415596fa4d9afe442b2c19e61f",
			 "c56cac07a500e5864a994b19286bc07c",
			 NULL}
	},
	{"borderzone", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
		{"189231ed0675f6be3be86856f49211af", NULL}
	},
	{"bureaucracy", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"a8ae194257a989ed3d82648a507466f2",
			 "2bb00311d4c201082cfcd278ae5db921",
			 NULL}
	},
	{"cutthroats", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"059801d9f90fffeb3645816c37c7eda2", NULL}
	},
	{"deadline", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"1610e84ca2505885566e648c1c525976",
			 "e1ae6af1098067b86076c34865ae713c",
			 "166ffb7cabc6b85f210655f371c89c46",
			 NULL}
	},
	{"enchanter", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"7b41d915b4c2e31423d99925e9438aa4",
			 "e70f21aad650dd196fa3601cab5e0fc5",
			 "46187e0691f6f5ecdd5a336885db6aad",
			 "f87cdafad3682ead25cfc473656ff713",
			 NULL}
	},
	{"hhgttg", Gargoyle::GID_ZCODE_HHGTTG, Common::EN_ANY,
			{"fdda8f4239819402c62db866bb61a648",
			 "a214fcb42bc9f554d07d983a12f6a062",
			 "e867d49ad1fb9406ff4e0678a4ee2ac9",
			 "34f6abc1f2a42be127ef434fc475f0ee", 
			 "379022bcd4ec74b90274c6100c33f579",
			 NULL}
	},
	{"hijinx", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"7b52824057ae24e098c228c41460ef75", NULL}
	},
	{"infidel", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"38f713e53af720624434529ea780040c", NULL}
	},
	{"journey", Gargoyle::GID_ZCODE_JOURNEY, Common::EN_ANY,
			{"c9893bc0399080bd3850d4db2120d110",
			 "8a4ab56f62e1b7c918b837794182dbcd",
			 "c33ea33ab8aec6c617734dcfe1211067",
			 NULL}
	},
	{"lgop", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"69b3534570851b90d7f53ebe9d224a6a",
			 "6bdae7434df7c03f3589ece0bed3317d",
			 "e81237e220a612c5a93fbcc1fdf85a0a",
			 NULL}
	},
	{"lurking", Gargoyle::GID_ZCODE_LURKING_HORROR, Common::EN_ANY,
			{"e2d2505510479fec0405727e3d0abc10",
			 "83936d75c2cfd71fb64bf63c4696b9ac",
			 "c60cd0bf3c6eda867241378c7cb5464a",
			 NULL}
	},
	{"minizork1", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"0d7700679e5e63dec97f712698610a46", NULL}
	},
	{"moonmist", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"284797c3025ffaf76aecfa5c2bbffa86",
			 "698475de2769c66bc5a1eca600c71561",
			 NULL}
	},
	{"nordbert", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"da1e189e19e3b24b2e35bd41fc32d261", NULL}
	},
	{"planetfall", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"34c69f1d24418fd4d2de195a1d7546c4",
			 "15815c461a8548b7630d2aee46d07cc7",
			 "cf6ce61eb2eff9d4f18d7bcba7c12cfb",
			 "9facd8b974e658520fb762af4c4789dc",
			 "01844816673414c97d21dc003304989b",
			 NULL}
	},
	{"plundered", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"fe5b9eb91949d41838166364f1753b10", NULL}
	},
	{"sampler1", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"5483febc51abd55fb5e04c4c97a0b260",
			 "47b8b8394e25faec870a798145529688",
			 "05d9d1a1c3c73fce9e24ab695ece16c8",
			 NULL}
	},
	{"sampler2", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"201fa230a942df5aa75bb5b5f609e8ce", NULL}
	},
	{"seastalker", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"2f0220b0390deda695e01832a92b5493",
			 "050961fa7788c309bbf40accbff2ffdf",
			 "bccf194b1e823e37db2431b586662773",
			 "eb39dff7beb3589c8581dd2e3569eb78",
			 "64fb27e7b9fd682ff4f0d0ec6616a468",
			 NULL}
	},
	{"sherlock", Gargoyle::GID_ZCODE_SHERLOCK, Common::EN_ANY,
			{"69862f7f07a4e977159ea4da7f2f2ba6",
			 "2cb2bda2e34eb7f9494cb585720e74cd",
			 NULL}
	},
	{"shogun", Gargoyle::GID_ZCODE_SHOGUN, Common::EN_ANY,
			{"62cca41feb94082442026f44f3e48e19", NULL}
	},
	{"sorcerer", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"7a076459806eaee72015b2b2882a89dc",
			 "cde88a011d2ba183ff69b47b0d8881c6",
			 "d4a914fdfe90f5cd055a03b9aa24addd",
			 "7ee357c10a9e049fe7c641a4817ee575",
			 NULL}
	},
	{"spellbreaker", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"b7b9eef231dee03fb40a9d98416fa0d5",
			 "852286847f4cdd790075fa824260ff4e",
			 NULL}
	},
	{"starcross", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"fb2e6d9a0ad5822f3a8d4aec949e4e3c",
			 "ed1e62e1f0eb9d819be45c076c5729f7",
			 NULL}
	},
	{"stationfall", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"cfadfb66afabaa2971ec9b4ae65266ca", NULL}
	},
	{"suspect", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"3d759ccb19233f51968fa79d7374b393", NULL}
	},
	{"suspended", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"d898430e3cccdee9f9acfffcc9ef709c",
			 "65f0cc760a2500d110242fbf942f1028",
			 "b749d42462dfec21831b69635cd9c5e8",
			 "6088ad7cb553626b52875a9b8e801312",
			 NULL}
	},
	{"trinity", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"994ea591f8d401e11661c912b92ee05e",
			 "5377dc1ee39f1c8ed572944f89946eb2",
			 NULL}
	},
	{"wishbringer", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"bec823084c5622e88eca5a886278d2a5",
			 "898b9b157ce8e54a0953366d6317fbd5",
			 "e7c0412c4b3bda39de438a02cbae3816",
			 NULL}
	},
	{"witness", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"d2297ddfe2c1b976c1b0c381ab01e2b3",
			 "a6e441b0b92a72537c830fed201267af",
			 "1019b9b1e1aa2c6eda945d7d92c2073a",
			 NULL}
	},
	{"zork0", Gargoyle::GID_ZCODE_ZORK_ZERO, Common::EN_ANY,
			{"b58c35dc2ba36d48fade99564922c7c3",	// demo version
			 "fca554c21542729c9d154c319af6307e",
			 "e787b2cad2d6f29fd812e737f75646e8",
			 "32e3e7ec438dabe77df2351af6ece324",
			 "29fb0e090bbff7bc8e9661e55da69ae7",
			 NULL}
	},


	{"zork1", Gargoyle::GID_ZCODE_ZORK1, Common::EN_ANY,
			{"b1fa36d05b2a7379d83d843118bd087c",
			 "fa2d22304700898cb8de921f46ca4bc9",
			 "b222bed4a0ab2650135cba7f4b1b1c67",
			 "6ad3d9ab2874beefcbc53630e9261118",
			 "287a1ce17f458fb2e776499a13796719",
			 "285f1d7c5deb1a2f23825f63823d0777",
			 "83bb70d73f3b4b5c4a32d8588b2d0707",
			 "d6d8b3ae49a683a6fce2383a8bab36a5",
			 "dd5ba502b30189d03abfcfb9817dffe0",
			 "e56267fd041c71fc229f7deb6b6537c2",
			 "b35bca8dd18f6312c7e54dcd7958d7e5",
			 "50ebf3c0c959ac2571c23cb7f7907c70",
			 "d708b6751126f3b2b7612c760f080d41",
			 NULL}
	},
	{"zork1", Gargoyle::GID_ZCODE_ZORK1, Common::DE_DEU,
			{"9f336c92c1fd392fc7e81288e5c9b6ab", NULL}
	},
	{"zork2", Gargoyle::GID_ZCODE_ZORK2, Common::EN_ANY,
			{"4b6ecc8e40243ddbd4cc19ef82304c3b",
			 "386f2cd937e0ca316695d6ddca521c78",
			 "a019dd721134b57f5926ee2adf634b55",
			 "6cafa0e5239a74aa120bb8e2c33441be",
			 "a5236020509af26b47c737e51ce884aa",
			 "600264d62720731283454592261ec3fe",
			 "6c2e766b553c127bb07f7a0f8fe03ae2",
			 "a5064c9c3ce0bc02f16e01d745f39b67",
			 "8243ce12e7b3ce24b150f34cc2cb472c",
			 NULL}
	},
	{"zork3", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"ba4897f4d82ba08906295dd3aebf501a",
			 "2fb29e6f5eebb643f42714ca9086e145",
			 "672b54d8f457bd3be32e41fc9e069d71",
			 "4717f8ec2f08da7d438c05f1351d28bd",
			 "c5dc520f469771c59d193558d405341d",
			 NULL}
	},
	{"zorkuu", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{"3a55991be19943a13da58a91cd3615eb", NULL}
	},

	// Miscellaneous
	{"freefall", Gargoyle::GID_ZCODE_UNKNOWN, Common::EN_ANY,
			{(const char *)"3e7898a1e767a2de61336ff2ff4bb1f4", NULL}
	},

	{NULL, (Gargoyle::GargoyleGameId)0, Common::UNK_LANG, NULL, NULL }
};

bool FrotzInterpreter::validateFile(const Common::FSNode &file, char *target, char *version, Common::Language &lang) {
	Common::File f;
	Gargoyle::BlorbFile *blorbFile = NULL;
	Common::SeekableReadStream *rs = NULL;
	bool isValid;

	strcpy(target, "");
	strcpy(version, "");

	Common::String filename = file.getName();
	filename.toLowercase();
	const char *p = strchr(filename.c_str(), '.');
	if (!p) return false;		// Files without extension can't be Frotz games
	++p;

	// Check for '.z1' through '.z8', '.zip', or '.dat' file
	bool knownType1 = ((*p == 'z') && (*(p + 1) >= '1') && (*(p + 1) <= '8') && (*(p + 2) == '\0'));
	bool knownType2 = !strcmp(p, "zip") || !strcmp(p, "dat");

	if (knownType1 || knownType2) {
		// Known filetype for z-code gamefile
		// Open up the file for access, and get the first 5000 bytes
		f.open(file);
		rs = f.readStream(5000);

	} else {
		// Secondary fallback - check if it's a Blorb file
		if ((strcmp(p, "blb") != 0) && (strcmp(p, "blorb") != 0) && (strcmp(p, "zblorb") != 0) &&
			(strcmp(p, "zlb") != 0))
			return false;

		// Open the file as a Blorb file
		if (!f.open(file))
			error("Unable to open file");
		blorbFile = new Gargoyle::BlorbFile();
		if (blorbFile->open(f.readStream(f.size())) == Gargoyle::bb_err_None) {
			// Look for an executable chunk
			Gargoyle::bb_result_t result;
			if (blorbFile->loadResource(bb_method_FilePos, &result, bb_ID_Exec, 0) == Gargoyle::bb_err_None) {
				// Has one, check it's Id
				unsigned int id = blorbFile->chunks[result.chunknum]->type;
				if (id == bb_make_id('Z','C','O','D')) {
					// Found game file, so get it separately
					blorbFile->stream()->seek(result.data.startpos);
					rs = blorbFile->stream()->readStream(MIN((int)result.length, 5000));
				} 
			}
		}

		if (!rs) {
			delete blorbFile;
			return false;
		}
	}

	// Get an MD5 for the file and scan through the Z-code game list
	char md5Str[32 + 1];

	if (!Common::md5_file_string(*rs, md5Str, 5000)) {
		delete rs;
		if (blorbFile) delete blorbFile;
		return false;
	}

	// Scan through the list for the MD5 of the game
	for (const FrotzGameEntry *gs = &frotz_games[0]; gs->target; ++gs) {
		const char * const*md5P = gs->md5;
		while ((*md5P != NULL) && (strcmp(*md5P, md5Str) != 0))
			++md5P;

		if (*md5P) {
			strcpy(target, gs->target);
			lang = gs->lang;
			break;
		}
	}

	bool unknownFlag = !strcmp(target, "");
	if (unknownFlag) {
		// Do further checks here for a generic z-code gamefile
		char buffer[16];
		rs->seek(0);
		isValid = (rs->read(buffer, 16) == 16);
		if (isValid)
			isValid = (buffer[0] >= 1) && (buffer[0] <= 8);

		if (isValid) {
			// Is a valid, but unknown, game, so use default z-code target
			strcpy(target, "zcode");
			lang = Common::EN_ANY;
		}

	} else {
		// Game is already known to be valid
		isValid = true;
	}

	if (isValid) {
		char buffer[10];
		// Get the game release
		rs->seek(2);
		uint16 v = rs->readUint16BE();
		// Get the version string
		rs->seek(18);
		rs->read(buffer, 6);
		buffer[6] = 0;
		for (int i = 0; i < 6; ++i)
			if ((buffer[i] < ' ') || (buffer[i] >= 'z'))
				buffer[i] = 0;

		// Form final description
		if (v == 0)
			strcpy(version, "");
		else {
			sprintf(version, "%d", v);
			if (strlen(buffer) > 0)
				strcat(version, ", ");
		}

		if (strlen(buffer) > 0)
			strcat(version, buffer);

		if (unknownFlag)
			warning("Unknown z-code game '%s' (%s)\nmd5=%s", filename.c_str(), version, md5Str);
	}
	delete rs;
	if (blorbFile) delete blorbFile;

	return isValid;
}

bool FrotzInterpreter::identifyGame(const Common::String &target, Gargoyle::GargoyleGameId &gameId) {
	// Check for the generic 'zcode' target
	if (!strcmp(target.c_str(), "zcode")) {
		gameId = Gargoyle::GID_ZCODE_UNKNOWN;
		return true;
	}

	// Scan through the game list for an entry
	for (const FrotzGameEntry *gs = &frotz_games[0]; gs->md5; ++gs) {
		if (!strcmp(gs->target, target.c_str())) {
			gameId = gs->gameId;
			return true;
		}
	}

	// It's not a z-code game
	return false;
}

} // end of namespace Frotz
