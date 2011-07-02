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
 
void PuzzlePipes::Peephole::connect(ConnectorPtr connector) {
	_connectors.push_back(connector);
}

void PuzzlePipes::Peephole::disconnect(ConnectorPtr connector) {
	_connectors.remove(connector);
}

void PuzzlePipes::Peephole::startUpWater(bool flag = false) {
	if (flag)
		memset(marks, false, sizeof(marks));

	marks[_id] = true;
	for (Common::List<ConnectorPtr>::iterator iter = _connectors.begin();
	iter != _connectors.end(); ++iter)
		for (Peepiter iter1 = (*iter)->_connectedNodes.begin();
		iter1 != (*iter)->_connectedNodes.end(); ++iter1)
			if (!marks[(*iter1)->getId()]) {
				for (uint32 i = 0; i < 4; ++i)
					if (isConnected(i) && (*iter1)->getId() > 3)
						(*iter1)->_flowValues[i] += _flowValues[i];
				(*iter1)->startUpWater();
			}
}

void PuzzlePipes::Connector::init(Peephole *n, Peephole *e, Peephole *s, Peephole *w,
BinNum state, ConnectorPtr nextConnector = NULL, Direction nextConnectorPosition = kNowhere) {
			_nodes[0] = n;
	_nodes[3] = w;			_nodes[1] = e;
			_nodes[2] = s;
	
	_state = state;
	_nextConnector = nextConnector;
	_nextConnectorPosition = nextConnectorPosition;
	_isConnected = false;

	for (uint32 i = 0; i < 4; ++i)
		if (_state & (1 << i) && _nodes[i]) {
			_nodes[i]->connect(this);
			_connectedNodes.push_back(_nodes[i]);
		}
}

void PuzzlePipes::Connector::initGroup() {
	if (!_isConnected && isReadyForConnection() && _nextConnector->isReadyForConnection())
		connect(_nextConnector);
}

void PuzzlePipes::Connector::turn() {
	BinNum newState = BinNum(_state >> 1 | (_state & 1) << 3);

	uint32 delta = _state ^ newState;
	uint32 newIndex[2], oldIndex[2];

	if (delta == k1111) {
		if (newState == k0101) {
			newIndex[0] = 0;
			newIndex[1] = 2;
			oldIndex[0] = 1;
			oldIndex[1] = 3;
		} else {
			newIndex[0] = 1;
			newIndex[1] = 3;
			oldIndex[0] = 0;
			oldIndex[1] = 2;
		}
	} else {
		newIndex[0] = log2(newState & delta);
		oldIndex[0] = log2(_state & delta);
	}

	for (uint32 i = 0; i < (delta == k1111)  + 1; ++i) {
		if (_nodes[oldIndex[i]]) {
			_nodes[oldIndex[i]]->disconnect(this);
			_connectedNodes.remove(_nodes[oldIndex[i]]);
		}

		if (_nodes[newIndex[i]]) {
			_nodes[newIndex[i]]->connect(this);
			_connectedNodes.push_back(_nodes[newIndex[i]]);
		}
	}

	_state = newState;

	if (_nextConnector) {
		if (_isConnected) {
			if(!(_nextConnectorPosition & _state))
				disconnect(_nextConnector);
		} else if (_nextConnectorPosition & _state && _nextConnector->isReadyForConnection())
			connect(_nextConnector);
	}
}

void PuzzlePipes::Connector::connect(ConnectorPtr connector) {
	Peepiter iter;

	for (iter = _connectedNodes.begin(); iter != _connectedNodes.end(); ++iter) {
		(*iter)->connect(connector);
		connector->_connectedNodes.push_back(*iter);
	}

	for (iter = connector->_connectedNodes.begin(); iter != connector->_connectedNodes.end(); ++iter) {
		(*iter)->connect(this);
		_connectedNodes.push_back(*iter);
	}

	_isConnected = connector->_isConnected = true;

}

void PuzzlePipes::Connector::disconnect(ConnectorPtr connector) {
	int32 i;
	Common::List<Peepiter> markedForDeletion;
	bool flag;

	for (i = 0; i < 4; ++i)
		if (_nodes[i]) {
			_nodes[i]->disconnect(connector);
			connector->_connectedNodes.remove(_nodes[i]);
		}

	for (Peepiter iter = _connectedNodes.begin(); iter != _connectedNodes.end(); ++iter) {
		flag = true;

		for (i = 0; i < 4; ++i)
			if (*iter == _nodes[i]) {
				flag = false;
				break;
			}

		if (flag)
			markedForDeletion.push_back(iter);
	}

	for (Common::List<Peepiter>::iterator iter1 = markedForDeletion.begin(); iter1 != markedForDeletion.end(); ++iter1)
	{
		_connectedNodes.remove(*(*iter1));
		(*(*iter1))->disconnect(this);
	}

	_isConnected = connector->_isConnected = false;
}