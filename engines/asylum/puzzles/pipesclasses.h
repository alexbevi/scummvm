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
 */

#ifndef ASYLUM_PIPESCLASSES_H
#define ASYLUM_PIPESCLASSES_H

#include "asylum/puzzles/puzzle.h"

namespace Asylum {

const uint32 connectorsCount = 21, peepholesCount = 37;

enum BinNum {
	kBinNum0000,
	kBinNum0001,
	kBinNum0010,
	kBinNum0011,
	kBinNum0100,
	kBinNum0101,
	kBinNum0110,
	kBinNum0111,
	kBinNum1000,
	kBinNum1001,
	kBinNum1010,
	kBinNum1011,
	kBinNum1100,
	kBinNum1101,
	kBinNum1110,
	kBinNum1111
};

enum Direction {
	kDirectionNowhere = kBinNum0000,
	kDirectionNh = kBinNum0001,
	kDirectionEt = kBinNum0010,
	kDirectionSh = kBinNum0100,
	kDirectionWt = kBinNum1000
};

class Peephole;
class Connector;
class Spider;	// TODO
typedef Connector * ConnectorPtr;
typedef Common::List<Peephole *>::iterator Peepiter;

class Peephole {
public:
	static bool marks[peepholesCount];
	uint32 _flowValues[4];

	Peephole() { }
	~Peephole() { }

	uint32 getId() { return _id; }
	void setId(uint32 id) { _id = id; }
	uint32 getLevel() { return (_flowValues[0] > 0) + (_flowValues[1] > 0) + (_flowValues[2]  > 0) + (_flowValues[3]  > 0); }
	uint32 getLevel1() { return _flowValues[0] + _flowValues[1] + _flowValues[2] + _flowValues[3]; }
	bool isConnected() { return isConnected(0) || isConnected(1) || isConnected(2) || isConnected(3); }

	void connect(ConnectorPtr);
	void disconnect(ConnectorPtr);
	void startUpWater(bool flag);
private:
	uint32 _id;
	Common::List<ConnectorPtr> _connectors;

	bool isConnected(uint32 val) { return _flowValues[val]; }
};

class Connector {
public:	
	Connector() { }
	~Connector() { }

	uint32 getId() { return _id; }
	void setId(uint32 id) { _id = id; }
	BinNum getState() { return _state; }

	void init(Peephole *, Peephole *, Peephole *, Peephole *, BinNum, ConnectorPtr nextConnector = NULL, Direction nextConnectorPosition = kDirectionNowhere);
	void initGroup();
	void turn(); 

	friend void Peephole::startUpWater(bool);
private:
	uint32 _id;
	BinNum _state;
	Peephole *_nodes[4];
	Common::List<Peephole *> _connectedNodes;

	ConnectorPtr _nextConnector;
	Direction _nextConnectorPosition;
	bool _isConnected;

	void connect(ConnectorPtr conn); 
	void disconnect(ConnectorPtr conn);

	bool isReadyForConnection() { return _state & _nextConnectorPosition; }
};

} // End of namespace Asylum

#endif // ASYLUM_PIPESCLASSES_H
