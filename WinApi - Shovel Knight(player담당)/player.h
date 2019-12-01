#pragma once
#include "gameNode.h"
#include "bullet.h"

class objects;
class NPC;
class enemy;

enum PLAYERMAINCONDITION       // �ɸ����� ���� ���°� ������ ���� �̳ѹ�
{
	PLAYER_RIGHT_IDLE,  		  // ������ ������
	PLAYER_LEFT_IDLE,			  // ���� ������ 
	PLAYER_IDLE_JUMP,			  // ���ڸ� ����
	PLAYER_RIGHT_JUMP,			  // ������ ����
	PLAYER_LEFT_JUMP,			  // ���� ����
	PLAYER_RIGHT_MOVE,			  // ������ �̵�
	PLAYER_LEFT_MOVE,			  // ���� �̵�
	PLAYER_UP_CLIMB,			  // ��ٸ� Ÿ���� ������ ��������
	PLAYER_DOWN_CLIMB,			  // ��ٸ� Ÿ���� ����Ű ��������
	PLAYER_EDGE_CLIMB,			  // ��ٸ� ��� ���ڶ��� ��ģ����
	PLAYER_RIGHT_ATTACK,		  // ������ ���� 
	PLAYER_RIGHT_JUMP_ATTACK,	  // ������ ��������
	PLAYER_LEFT_ATTACK,			  // ���� ���� 
	PLAYER_LEFT_JUMP_ATTACK,	  // ���� ��������
	PLAYER_DOWN_ATTACK,			  // �Ʒ� ����
	PLAYER_RIGHT_DOWN_ATTACK,
	PLAYER_LEFT_DOWN_ATTACK,
	PLAYER_RIGHT_HITTED,		  // �����ʿ����� �ǰ�
	PLAYER_LEFT_HITTED,			  // ���ʿ����� �ǰ�
	PLAYER_DEAD,				  // ����

};

enum PLAYERSUBCONDITION		  // �÷��̾� ���� ���°��� ����  ex) npc�� ��ٸ��� ��Ʈ �浹 �� ���¿��� ����Ű�� ���� ��� Ʈ���� �뵵
{
	PLAYER_NOTHING,
	PLAYER_NPC,
	PLAYER_LADDER,
};
class player : public gameNode
{
	PLAYERMAINCONDITION _playerMainCondition;
	PLAYERSUBCONDITION _playerSubCondition;
	image* _image;
	animation* _ani;
	RECT _imageRC;
	RECT _playerRC;
	RECT _attackRC;
	int _maxHP;
	int _currentHP;
	int _maxMP;
	int _currentMP;
	int _playerGold;
	int _equipmentRelic;
	int _dir;
	float _speed;
	float _jumpPower;
	float _gravity;
	float _x;
	float _y;
	float _probeY;
	float _repulsivePower;      // Ÿ�� �� �÷��̾ �ڷ� �ڿ������� �о�� ���� �ݹ߷�
	float _frictionalPower;		// �ݹ߷��� ������ �谨��Ű�� ���� ������
	bool _isLadder;
	bool _isLand;
	bool _isJump;
	bool _canAtk;
	bool _offPicxel;
	bool _immune;
	bool _video;
	int prevCondition1;
	int prevCondition2;

	bool _objectLanding;
	RECT* _landingObject;
	float* _landingObjectSpeedX;
	float* _landingObjectSpeedY;

	bullet* _Relic;
	int _currentRelic;
	float _bulletAngle;
public:
	player();
	~player();

	RECT LadderRC;
	RECT enemyRC;

	HRESULT init();
	void release();
	void update();
	void render();
	void usage();
	void setPlayerCondition();
	void collisonAttack();
	void collisonHitted();
	
	
	

	static void rightAttack(void* obj);
	static void leftAttack(void* obj);
	static void rightJumpAttack(void* obj);
	static void leftJumpAttack(void* obj);
	//static void downAttack(void* obj);
	bool getOffPicxel() { return _offPicxel; }

	/////////// ĳ������ ���λ��� ������, ������ 
	PLAYERMAINCONDITION getPlayerMainCondition(void) { return _playerMainCondition; }
	void setPlayerMainCondition(PLAYERMAINCONDITION condition) { _playerMainCondition = condition; }

	//////////// ĳ������ ������� ������ ������
	PLAYERSUBCONDITION getPlayerSubCondition(void) { return _playerSubCondition; }
	void setPlayerSubCondition(PLAYERSUBCONDITION condition) { _playerSubCondition = condition; }

	///////////  �÷��̾� �̹��� ������
	void setPlayerImage(image* ima) { _image = ima; }
	void setPlayerAni(animation* ani) { _ani = ani; }

	image* getPlayerImage(void) { return _image; }
	animation* getPlayerAni(void) { return _ani; }


	////////  �÷��̾� �浹��Ʈ ������ ������
	RECT* getPlayerRect() { return &_playerRC; }
	RECT* getPlayerAttackRect() { return &_attackRC; }
	void setPlayerAttackRect(RECT _rc) { _attackRC = _rc; }
	//////// ���� ü�� ������ ������

	int getPlayerCurrentHP() { return _currentHP; }
	void setPlayerCurrentHP(int hp) { _currentHP = hp; }

	//////// �ִ� ü�� ������ ������
	int getPlayerMaxHP() { return _maxHP; }
	void setPlayerMaxHP(int hp) { _maxHP = hp; }


	/////// ���� ����  ������ ������
	int getPlayerCurrentMP() { return _currentMP; }
	void setPlayerCurrentMP(int mp) { _currentMP = mp; }


	/////// �ִ� ���� ������ ������  
	int getPlayerMaxMP() { return _maxMP; }
	void setPlayerMaxMP(int mp) { _maxMP = mp; }


	/////// ���  ������ ������
	int getPlayerGold() { return _playerGold; }
	void setPlayerGold(int gold) { _playerGold = gold; }


	//////  ������ ������ ������ȣ ������ ������
	int getEquipRelic() { return _equipmentRelic; }
	void setEquipRelic(int relicValue) { _equipmentRelic = relicValue; }


	// ��������

	bool getIsJump() { return _isJump; }
	void setIsJump(bool boolValue)
	{
		//_isJump = boolValue;
		if (boolValue)
		{
			switch (_playerMainCondition)
			{
			case PLAYER_RIGHT_IDLE:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				setPlayerCondition();
				break;
			case PLAYER_LEFT_IDLE:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				setPlayerCondition();
				break;
			case PLAYER_RIGHT_MOVE:
				_playerMainCondition = PLAYER_RIGHT_JUMP;
				setPlayerCondition();
				break;
			case PLAYER_LEFT_MOVE:
				_playerMainCondition = PLAYER_LEFT_JUMP;
				setPlayerCondition();
				break;
			case PLAYER_UP_CLIMB:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				setPlayerCondition();
				break;
			case PLAYER_DOWN_CLIMB:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				setPlayerCondition();
				break;
			case PLAYER_EDGE_CLIMB:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				setPlayerCondition();
				break;
			}
		}
			else if (!boolValue)
			{
				switch (_playerMainCondition)
				{
					case PLAYER_IDLE_JUMP:
					if (_dir == 1) 	_playerMainCondition = PLAYER_RIGHT_IDLE;
					if (_dir == -1)	_playerMainCondition = PLAYER_LEFT_IDLE; 
					setPlayerCondition();
					break;
					case PLAYER_RIGHT_JUMP:
					_playerMainCondition = PLAYER_RIGHT_MOVE;
					setPlayerCondition();
					break;
					case PLAYER_LEFT_JUMP:
					_playerMainCondition = PLAYER_LEFT_MOVE;
					setPlayerCondition();
					break;

					case PLAYER_DOWN_ATTACK:
					if (_dir == 1) 	_playerMainCondition = PLAYER_RIGHT_IDLE;
					if (_dir == -1)	_playerMainCondition = PLAYER_LEFT_IDLE;
					setPlayerCondition();
					break;

					case PLAYER_RIGHT_DOWN_ATTACK:
						_playerMainCondition = PLAYER_RIGHT_IDLE;
						setPlayerCondition();
						break;

					case PLAYER_LEFT_DOWN_ATTACK:
						_playerMainCondition = PLAYER_LEFT_IDLE;
						setPlayerCondition();
						break;
				}

			}

		_isJump = boolValue;
	}
	
	void setIsLand(bool landvalue) { _isLand = landvalue; }
	bool getIsLand() { return _isLand; }
	// �����Ŀ� ������

	float getJumpPower() { return _jumpPower; }
	void setJumpPower(float jumppower) { _jumpPower = jumppower; }

	// Ž���� ������  ������
	void setProbeY(float probe) { _probeY = probe; }
	float getprobeY() { return _probeY; }


	// �÷��̾� ��ǥ ���� ������

	void setPlayerX(float x) { _x = x; }
	float getPlayerX() { return _x; }
	void setPlayerY(float y) { _y = y; }
	float getPlayerY() { return _y; }

	void getColMessage(LPCOLLISION_INFO message);

};














enum usg
{
	FIRELOD,
	ORB,
	FISHINGROD
};