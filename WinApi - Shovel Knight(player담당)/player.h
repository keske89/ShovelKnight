#pragma once
#include "gameNode.h"
#include "bullet.h"

class objects;
class NPC;
class enemy;

enum PLAYERMAINCONDITION       // 케릭터의 메인 상태값 설정을 위한 이넘문
{
	PLAYER_RIGHT_IDLE,  		  // 오른쪽 가만히
	PLAYER_LEFT_IDLE,			  // 왼쪽 가만히 
	PLAYER_IDLE_JUMP,			  // 제자리 점프
	PLAYER_RIGHT_JUMP,			  // 오른쪽 점프
	PLAYER_LEFT_JUMP,			  // 왼쪽 점프
	PLAYER_RIGHT_MOVE,			  // 오른쪽 이동
	PLAYER_LEFT_MOVE,			  // 왼쪽 이동
	PLAYER_UP_CLIMB,			  // 사다리 타는중 오른쪽 누른상태
	PLAYER_DOWN_CLIMB,			  // 사다리 타는중 왼쪽키 누른상태
	PLAYER_EDGE_CLIMB,			  // 사다리 상단 끝자락에 걸친상태
	PLAYER_RIGHT_ATTACK,		  // 오른쪽 공격 
	PLAYER_RIGHT_JUMP_ATTACK,	  // 오른쪽 점프공격
	PLAYER_LEFT_ATTACK,			  // 왼쪽 공격 
	PLAYER_LEFT_JUMP_ATTACK,	  // 왼쪽 점프공격
	PLAYER_DOWN_ATTACK,			  // 아래 공격
	PLAYER_RIGHT_DOWN_ATTACK,
	PLAYER_LEFT_DOWN_ATTACK,
	PLAYER_RIGHT_HITTED,		  // 오른쪽에서의 피격
	PLAYER_LEFT_HITTED,			  // 왼쪽에서의 피격
	PLAYER_DEAD,				  // 죽음

};

enum PLAYERSUBCONDITION		  // 플레이어 서브 상태값을 위한  ex) npc나 사다리에 렉트 충돌 된 상태에서 방향키에 따른 기능 트리거 용도
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
	float _repulsivePower;      // 타격 시 플레이어를 뒤로 자연스럽게 밀어내기 위한 반발력
	float _frictionalPower;		// 반발력을 서서히 삭감시키기 위한 마찰력
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

	/////////// 캐릭터의 메인상태 접근자, 설정자 
	PLAYERMAINCONDITION getPlayerMainCondition(void) { return _playerMainCondition; }
	void setPlayerMainCondition(PLAYERMAINCONDITION condition) { _playerMainCondition = condition; }

	//////////// 캐릭터의 서브상태 접근자 설정자
	PLAYERSUBCONDITION getPlayerSubCondition(void) { return _playerSubCondition; }
	void setPlayerSubCondition(PLAYERSUBCONDITION condition) { _playerSubCondition = condition; }

	///////////  플레이어 이미지 설정자
	void setPlayerImage(image* ima) { _image = ima; }
	void setPlayerAni(animation* ani) { _ani = ani; }

	image* getPlayerImage(void) { return _image; }
	animation* getPlayerAni(void) { return _ani; }


	////////  플레이어 충돌렉트 접근자 설정자
	RECT* getPlayerRect() { return &_playerRC; }
	RECT* getPlayerAttackRect() { return &_attackRC; }
	void setPlayerAttackRect(RECT _rc) { _attackRC = _rc; }
	//////// 현재 체력 접근자 설정자

	int getPlayerCurrentHP() { return _currentHP; }
	void setPlayerCurrentHP(int hp) { _currentHP = hp; }

	//////// 최대 체력 접근자 설정자
	int getPlayerMaxHP() { return _maxHP; }
	void setPlayerMaxHP(int hp) { _maxHP = hp; }


	/////// 현재 마나  접근자 설정자
	int getPlayerCurrentMP() { return _currentMP; }
	void setPlayerCurrentMP(int mp) { _currentMP = mp; }


	/////// 최대 마나 접근자 설정자  
	int getPlayerMaxMP() { return _maxMP; }
	void setPlayerMaxMP(int mp) { _maxMP = mp; }


	/////// 골드  접근자 설정자
	int getPlayerGold() { return _playerGold; }
	void setPlayerGold(int gold) { _playerGold = gold; }


	//////  장착된 유물의 고유번호 접근자 설정자
	int getEquipRelic() { return _equipmentRelic; }
	void setEquipRelic(int relicValue) { _equipmentRelic = relicValue; }


	// 점프여부

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
	// 점프파워 접근자

	float getJumpPower() { return _jumpPower; }
	void setJumpPower(float jumppower) { _jumpPower = jumppower; }

	// 탐사축 접근자  설정자
	void setProbeY(float probe) { _probeY = probe; }
	float getprobeY() { return _probeY; }


	// 플레이어 좌표 접근 설정자

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