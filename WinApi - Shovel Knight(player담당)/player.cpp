#include "stdafx.h"
#include "player.h"
#include "NPC.h"
#include "enemy.h"
#include "objects.h"


player::player()
{
}


player::~player()
{
}


HRESULT player::init()
{
	//IMAGEMANAGER->addFrameImage("playerWalk", "./image/character/playerWalk.bmp", 1500, 500, 6, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("playerClimb", "./image/character/playerLadder.bmp", 250, 750, 1, 3, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("playerIdle", "./image/character/playerIdle.bmp", 250, 500, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("playerHitted", "./image/character/playerHitted.bmp", 250, 500, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("playerDownAtk", "./image/character/playerDownAtk.bmp", 250, 500, 1, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("playerAtk", "./image/character/playerAtk.bmp", 1000, 500, 4, 2, true, RGB(255, 0, 255));
	//IMAGEMANAGER->addFrameImage("playerJump", "./image/character/playerJump.bmp", 500, 500, 2, 2, true, RGB(255, 0, 255));
	/*_Relic = new bullet;
	_Relic->init("파볼", 100, 800);*/
	
	//SOUNDMANAGER->addSound("공격", "./Music/삽공격.wav", false, false);
	//SOUNDMANAGER->addSound("광맥", "./Music/광맥히트.wav", false, false);
	//SOUNDMANAGER->addSound("큰벽", "./Music/큰벽.wav", false, false);

	_currentRelic = FIRELOD;
	_bulletAngle = PI;
	_playerMainCondition = PLAYER_RIGHT_IDLE;
	_playerSubCondition = PLAYER_NOTHING;

	LadderRC = RectMakeCenter(600, 450, 200, 600);

	enemyRC = RectMakeCenter(900, WINSIZEY / 2, 100, 100);


	_x = 2000;
	_y = 3800;

	_playerRC = RectMakeCenter(_x, _y, 80, 160);
	_imageRC = RectMakeCenter(_x, _y, 250, 250);
	_attackRC = RectMakeCenter(-100, -100, 150, 160);
	//플레이어 기본값 초기화
	_currentHP = _maxHP = 8;
	_currentMP = _maxMP = 30;
	_playerGold = 0;
	_equipmentRelic = NULL;
	_speed = 10.0f;
	_jumpPower = 8.00f;
	_gravity = 3.00f;
	_dir = 1;
	_probeY = 0;
	_repulsivePower = 3.0f;     // 타격 시 플레이어를 뒤로 자연스럽게 밀어내기 위한 반발력
	_frictionalPower = 0.2f;   // 반발력을 서서히 삭감시키기 위한 마찰력
	_isLadder = false;
	_isLand = false;
	_isJump = true;
	_canAtk = true;
	_offPicxel = false;
	_immune = false;
	_video = false;
	_objectLanding = false;
	

	int rightIdle[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("playerRightIdle", "playerIdle", rightIdle, 1, 6, true);
	int leftIdle[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("playerLeftIdle", "playerIdle", leftIdle, 1, 6, true);
	int rightMove[] = { 0,1,2,3,4,5 };
	KEYANIMANAGER->addArrayFrameAnimation("playerRightMove", "playerWalk", rightMove, 6, 8, true);
	int leftMove[] = { 11,10,9,8,7,6 };
	KEYANIMANAGER->addArrayFrameAnimation("playerLeftMove", "playerWalk", leftMove, 6, 8, true);
	int rightJumpUp[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("playerRightJumpUp", "playerJump", rightJumpUp, 1, 8, true);
	int rightJumpDown[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("playerRightJumpDown", "playerJump", rightJumpDown, 1, 8, true);
	int leftJumpUp[] = { 2 };
	KEYANIMANAGER->addArrayFrameAnimation("playerLeftJumpUp", "playerJump", leftJumpUp, 1, 8, true);
	int leftJumpDown[] = { 3 };
	KEYANIMANAGER->addArrayFrameAnimation("playerLeftJumpDown", "playerJump", leftJumpDown, 1, 8, true);
	int rightAttackarr[] = { 0,1,2,3 };
	KEYANIMANAGER->addArrayFrameAnimation("playerRightAttack", "playerAtk", rightAttackarr, 4, 10, false, rightAttack, this);
	int leftAttackarr[] = { 7,6,5,4 };
	KEYANIMANAGER->addArrayFrameAnimation("playerLeftAttack", "playerAtk", leftAttackarr, 4, 10, false, leftAttack, this);
	int leftDownAttack[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("playerLeftDownAttack", "playerDownAtk", leftDownAttack, 1, 8, true);
	int rightDownAttack[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("playerRightDownAttack", "playerDownAtk", rightDownAttack, 1, 8, true);
	int rightHitted[] = { 0 };
	KEYANIMANAGER->addArrayFrameAnimation("playerRightHitted", "playerHitted", rightHitted, 1, 8, true);
	int leftHitted[] = { 1 };
	KEYANIMANAGER->addArrayFrameAnimation("playerLeftHitted", "playerHitted", leftHitted, 1, 8, true);
	int climb[] = { 0,1 };
	KEYANIMANAGER->addArrayFrameAnimation("playerClimb", "playerClimb", climb, 2, 4, true);
	int edgeClimb[] = { 2 };
	KEYANIMANAGER->addArrayFrameAnimation("playerEdgeClimb", "playerClimb", edgeClimb, 1, 8, true);


	_image = IMAGEMANAGER->findImage("playerIdle");
	_ani = KEYANIMANAGER->findAnimation("playerRightIdle");

	DATABASE->setPlayer_currentInfo(_currentHP, _maxHP, _currentMP, _playerGold);
	return S_OK;
}
void player::release()
{
}
void player::update()
{

	///// 상태에 따른 중력 작용 여부 //////
	if (!_video)
	{
		if (_isJump)
		{
			_y -= _jumpPower;
			_jumpPower -= _gravity;
			_offPicxel = false;
		}
	}
	
	if (_isLand)
	{
		_jumpPower = 0.0f;
	}

	if (KEYMANAGER->isOnceKeyDown('K'))
	{
		_playerSubCondition = PLAYER_LADDER;
		_video = true;
	}

	if (KEYMANAGER->isOnceKeyDown('J'))
	{
		_playerSubCondition = PLAYER_NOTHING;
		_video = false;
	}



	////////////////////// 땅 위에서의 기본 동작 ///////////////////////////////

	if (_isLand && !_isJump && !_isLadder)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_dir = 1;
			_playerMainCondition = PLAYER_RIGHT_MOVE;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT) && _playerMainCondition != PLAYER_LEFT_MOVE)
		{
			_playerMainCondition = PLAYER_RIGHT_IDLE;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_dir = -1;
			_playerMainCondition = PLAYER_LEFT_MOVE;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT) && _playerMainCondition != PLAYER_RIGHT_MOVE)
		{
			_playerMainCondition = PLAYER_LEFT_IDLE;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{

			_objectLanding = false;
			_isJump = true;
			_jumpPower = 15.00f;
			_gravity = 0.65f;
			switch (_playerMainCondition)
			{
			case PLAYER_RIGHT_IDLE:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				break;
			case PLAYER_LEFT_IDLE:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				break;
			case PLAYER_IDLE_JUMP:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				break;
			case PLAYER_RIGHT_MOVE:
				_playerMainCondition = PLAYER_RIGHT_JUMP;
				break;
			case PLAYER_LEFT_MOVE:
				_playerMainCondition = PLAYER_LEFT_JUMP;
				break;
			case PLAYER_UP_CLIMB:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				break;
			case PLAYER_DOWN_CLIMB:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				break;
			case PLAYER_EDGE_CLIMB:
				_playerMainCondition = PLAYER_IDLE_JUMP;
				break;
			}
			setPlayerCondition();

		}
		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			//SOUNDMANAGER->play("공격");
			_canAtk = true;
			switch (_dir)
			{
			case 1:
				_playerMainCondition = PLAYER_RIGHT_ATTACK;
				setPlayerCondition();
				break;

			case -1:
				_playerMainCondition = PLAYER_LEFT_ATTACK;
				setPlayerCondition();
				break;

			}

		}

	}
	/////////// 사다리 렉트와 충돌 시 ////////////////////
	if (_playerSubCondition == PLAYER_LADDER)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_UP))
		{
			
			_playerMainCondition = PLAYER_UP_CLIMB;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_UP))
		{
			_ani->stop();
		}

		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			
			_playerMainCondition = PLAYER_DOWN_CLIMB;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_DOWN))
		{
			_ani->stop();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_SPACE))
		{
			//_y -= 3;
			//_playerRC = RectMakeCenter(_x, _y, 150, 160);
			switch (_dir)
			{
			case 1:
				_playerMainCondition = PLAYER_RIGHT_JUMP;
				setPlayerCondition();
				break;

			case -1:
				_playerMainCondition = PLAYER_LEFT_JUMP;
				setPlayerCondition();
				break;
			}
		}
	}
	/////////////////////  점프 상태에서 동작 ///////////////////////
	if (_isJump && _playerMainCondition != 14 && _playerMainCondition != 15 && _playerMainCondition != 16)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_playerMainCondition = PLAYER_RIGHT_JUMP;
			_dir = 1;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
		{
			_playerMainCondition = PLAYER_IDLE_JUMP;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_playerMainCondition = PLAYER_LEFT_JUMP;
			_dir = -1;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
		{
			_playerMainCondition = PLAYER_IDLE_JUMP;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
		{
			_canAtk = true;
			_playerMainCondition = PLAYER_DOWN_ATTACK;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyDown('A'))
		{
			SOUNDMANAGER->play("공격");
			_canAtk = true;
			switch (_dir)
			{
			case 1:
				_playerMainCondition = PLAYER_RIGHT_ATTACK;
				setPlayerCondition();
				break;

			case -1:
				_playerMainCondition = PLAYER_LEFT_ATTACK;
				setPlayerCondition();
				break;

			}

		}
	}
	else if (_playerMainCondition == PLAYER_DOWN_ATTACK
		|| _playerMainCondition == PLAYER_RIGHT_DOWN_ATTACK
		|| _playerMainCondition == PLAYER_LEFT_DOWN_ATTACK)
	{
		if (KEYMANAGER->isOnceKeyDown(VK_RIGHT))
		{
			_dir = 1;
			_playerMainCondition = PLAYER_RIGHT_DOWN_ATTACK;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_RIGHT))
		{
			_playerMainCondition = PLAYER_DOWN_ATTACK;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyDown(VK_LEFT))
		{
			_dir = -1;
			_playerMainCondition = PLAYER_LEFT_DOWN_ATTACK;
			setPlayerCondition();
		}
		if (KEYMANAGER->isOnceKeyUp(VK_LEFT))
		{
			_playerMainCondition = PLAYER_DOWN_ATTACK;
			setPlayerCondition();
		}

	}

	
	///////////////////////////// 플레이어 상태에 따른 이동 조작  /////////////////////////////////////

	switch (_playerMainCondition)
	{
	case PLAYER_RIGHT_IDLE:

		break;
	case PLAYER_LEFT_IDLE:

		break;
	case PLAYER_IDLE_JUMP:
		if (_jumpPower < 0 && _dir == 1) _ani = KEYANIMANAGER->findAnimation("playerRightJumpDown");
		if (_jumpPower < 0 && _dir == -1) _ani = KEYANIMANAGER->findAnimation("playerLeftJumpDown");
		break;
	case PLAYER_RIGHT_JUMP:
		if (_jumpPower < 0 ) _ani = KEYANIMANAGER->findAnimation("playerRightJumpDown");
		_x += _speed;
		break;
	case PLAYER_LEFT_JUMP:
		if (_jumpPower < 0 ) _ani = KEYANIMANAGER->findAnimation("playerLeftJumpDown");
		_x -= _speed;
		break;
	case PLAYER_RIGHT_MOVE:
		_x += _speed;
		break;
	case PLAYER_LEFT_MOVE:
		_x -= _speed;
		break;
	case PLAYER_UP_CLIMB:
		if (KEYMANAGER->isStayKeyDown(VK_UP))_y -= _speed;
		break;
	case PLAYER_DOWN_CLIMB:
		if (KEYMANAGER->isStayKeyDown(VK_DOWN))_y += _speed;
		break;
	case PLAYER_EDGE_CLIMB:
		break;
	case PLAYER_RIGHT_ATTACK:
		if (_canAtk)   _attackRC = RectMakeCenter(_x + 100, _y + 30, 75, 100);
		if (!_canAtk) _attackRC = RectMakeCenter(-150, 150, 100, 150);
		break;
	case PLAYER_RIGHT_JUMP_ATTACK:
		break;
	case PLAYER_LEFT_ATTACK:
		if (_canAtk)   _attackRC = RectMakeCenter(_x - 100, _y + 30, 75, 100);
		if (!_canAtk) _attackRC = RectMakeCenter(-150, 150, 100, 150);
		break;
	case PLAYER_LEFT_JUMP_ATTACK:
		break;
	case PLAYER_DOWN_ATTACK:
		if (_canAtk)   _attackRC = RectMakeCenter(_x, _y + 70, 100, 100);
		if (!_canAtk) _attackRC = RectMakeCenter(-150, 150, 100, 150);
		break;
	case PLAYER_RIGHT_DOWN_ATTACK:
		if (KEYMANAGER->isStayKeyDown(VK_RIGHT)) _x += _speed;
		if (_canAtk)   _attackRC = RectMakeCenter(_x, _y + 70, 100, 100);
		if (!_canAtk) _attackRC = RectMakeCenter(-150, 150, 100, 150);
		break;
	case PLAYER_LEFT_DOWN_ATTACK:
		if (KEYMANAGER->isStayKeyDown(VK_LEFT)) _x -= _speed;
		if (_canAtk)   _attackRC = RectMakeCenter(_x, _y + 70, 100, 100);
		if (!_canAtk) _attackRC = RectMakeCenter(-150, 150, 100, 150);
		break;
	case PLAYER_RIGHT_HITTED:
		_x -= _repulsivePower;
		_repulsivePower -= _frictionalPower;
		if (_repulsivePower <= 0)
		{
			_playerMainCondition = PLAYER_RIGHT_IDLE;
			setPlayerCondition();
		} 
		break;
	case PLAYER_LEFT_HITTED:
		_x += _repulsivePower;
		_repulsivePower -= _frictionalPower;
		if (_repulsivePower <= 0)
		{
			_playerMainCondition = PLAYER_LEFT_IDLE;
			setPlayerCondition();
		}
		break;
	case PLAYER_DEAD:
		break;

	}

	if (_objectLanding)
	{
		_x += *_landingObjectSpeedX;
		_y += *_landingObjectSpeedY;
		if (_x + 75 < _landingObject->left || _x - 75 > _landingObject->right)
		{
			_objectLanding = false;
			_isLand = false;
			_isJump = true;
		}
	}


	_playerRC = RectMakeCenter(_x, _y,80, 160);
	_imageRC = RectMakeCenter(_x, _y, 250, 250);
	if(!_canAtk && _playerMainCondition != PLAYER_RIGHT_ATTACK && _playerMainCondition != PLAYER_LEFT_ATTACK) _attackRC = RectMakeCenter(-150, 150, 100, 150);

	
	
	KEYANIMANAGER->update();
	//pixelCollison();
	//usage();
	//_Relic->update();
	DATABASE->setPlayer_currentInfo(_currentHP, _maxHP, _currentMP, _playerGold);
	//usage();
}


void player::render()
{
	Rectangle(getMemDC(), CAMERAMANAGER->CameraRelativePointX(LadderRC.left), CAMERAMANAGER->CameraRelativePointY(LadderRC.top), CAMERAMANAGER->CameraRelativePointX(LadderRC.right), CAMERAMANAGER->CameraRelativePointY(LadderRC.bottom));
	Rectangle(getMemDC(), CAMERAMANAGER->CameraRelativePointX(enemyRC.left), CAMERAMANAGER->CameraRelativePointY(enemyRC.top), CAMERAMANAGER->CameraRelativePointX(enemyRC.right), CAMERAMANAGER->CameraRelativePointY(enemyRC.bottom));

	_image->aniRender(getMemDC(), CAMERAMANAGER->CameraRelativePointX(_imageRC.left), CAMERAMANAGER->CameraRelativePointY(_imageRC.top), _ani);

	char str[128];
	sprintf(str, "케릭터 메인상태값 : %d", _playerMainCondition);
	TextOut(getMemDC(), 100, 100, str, strlen(str));

	char str1[128];
	sprintf(str1, "케릭터 서브상태값 : %d", _playerSubCondition);
	TextOut(getMemDC(), 100, 200, str1, strlen(str1));

	//_Relic->render();
	if (KEYMANAGER->isToggleKey(VK_F1))
	{
		//RectangleMake(getMemDC(), CAMERAMANAGER->CameraRelativePointX(_imageRC.left), CAMERAMANAGER->CameraRelativePointY(_imageRC.top),250,250);
		RectangleMake(getMemDC(), CAMERAMANAGER->CameraRelativePointX( _playerRC.left), CAMERAMANAGER->CameraRelativePointY(_playerRC.top), 80, 160);
		RectangleMake(getMemDC(), CAMERAMANAGER->CameraRelativePointX( _attackRC.left), CAMERAMANAGER->CameraRelativePointY(_attackRC.top),75, 100);
		sprintf(str, "attackRC LT %d %d RB %d %d", _attackRC.left, _attackRC.top, _attackRC.right, _attackRC.bottom);
		TextOut(getMemDC(), WINSIZEX / 2, WINSIZEY / 2, str, strlen(str));
	}

	char str2[128];
	sprintf(str2, " 스피드 : %0.2f", _speed);
	TextOut(getMemDC(), CAMERAMANAGER->CameraRelativePointX(_playerRC.left), CAMERAMANAGER->CameraRelativePointY(_playerRC.top) + 100, str2, strlen(str2));

	char str3[128];
	sprintf(str3, "케릭터 _isJump: %d", _isJump);
	TextOut(getMemDC(), CAMERAMANAGER->CameraRelativePointX(_playerRC.left), CAMERAMANAGER->CameraRelativePointY(_playerRC.top) + 120, str3, strlen(str3));

	char str4[128];
	sprintf(str4, "케릭터 점프파워: %0.2f", _jumpPower);
	TextOut(getMemDC(), CAMERAMANAGER->CameraRelativePointX(_playerRC.left), CAMERAMANAGER->CameraRelativePointY(_playerRC.top) + 140, str4, strlen(str4));

	char str5[128];
	sprintf(str5, "케릭터 _isLand: %d", _isLand);
	TextOut(getMemDC(), CAMERAMANAGER->CameraRelativePointX(_playerRC.left), CAMERAMANAGER->CameraRelativePointY(_playerRC.top) + 160, str5, strlen(str5));

	char str6[128];
	sprintf(str6, "canatk : %d", _canAtk);
	TextOut(getMemDC(), CAMERAMANAGER->CameraRelativePointX(_playerRC.left), CAMERAMANAGER->CameraRelativePointY(_playerRC.top) + 180, str6, strlen(str6));
}

void player::collisonAttack()
{
	_canAtk = false;
	_repulsivePower = 3.0f;
	_frictionalPower = 0.3f;
	


	if (_playerMainCondition >= 10 && _playerMainCondition <= 13)
	{
		switch (_dir)
		{
		case 1:
			_x -= _repulsivePower;
			_repulsivePower -= _frictionalPower;
			break;

		case -1:
			_x += _repulsivePower;
			_repulsivePower -= _frictionalPower;
			break;
		}
	}

	else if (_playerMainCondition >= 14 && _playerMainCondition <= 16)
	{
		_jumpPower = 11.0f;
		_gravity = 0.35f;
		_y -= _jumpPower;
		_jumpPower -= _gravity;
	}

	_canAtk = true;
}

void player::collisonHitted()
{
	
	_repulsivePower = 9.0f;
	_frictionalPower = 0.4f;
	
	switch (_dir)
	{
	case 1:
		_playerMainCondition = PLAYER_RIGHT_HITTED;
		setPlayerCondition();
		break;

	case -1:
		_playerMainCondition = PLAYER_LEFT_HITTED;
		setPlayerCondition();
		break;
	}
}


void player::rightAttack(void* obj)
{
	player* p = (player*)obj;

	if (!p->getIsJump())
	{
		p->setPlayerMainCondition(PLAYER_RIGHT_IDLE);
		p->setPlayerCondition();
	}
	else
	{
		p->setPlayerMainCondition(PLAYER_IDLE_JUMP);
		p->setPlayerCondition();
	}
}

void player::leftAttack(void * obj)
{
	player* p = (player*)obj;

	if (!p->getIsJump())
	{
		p->setPlayerMainCondition(PLAYER_LEFT_IDLE);
		p->setPlayerCondition();
	}

	else
	{
		p->setPlayerMainCondition(PLAYER_IDLE_JUMP);
		p->setPlayerCondition();
	}
}

void player::rightJumpAttack(void * obj)
{
}

void player::leftJumpAttack(void * obj)
{
}

void player::setPlayerCondition()
{
	switch (_playerMainCondition)
	{
	case PLAYER_RIGHT_IDLE:
		_canAtk = false;
		_immune = false;
		_image = IMAGEMANAGER->findImage("playerIdle");
		_ani = KEYANIMANAGER->findAnimation("playerRightIdle");
		_ani->start();
		break;
	case PLAYER_LEFT_IDLE:
		_canAtk = false;
		_immune = false;
		_image = IMAGEMANAGER->findImage("playerIdle");
		_ani = KEYANIMANAGER->findAnimation("playerLeftIdle");
		_ani->start();
		break;
	case PLAYER_IDLE_JUMP:
		_isLand = false;
		_isJump = true;
		_image = IMAGEMANAGER->findImage("playerJump");
		if (_dir == 1)   _ani = KEYANIMANAGER->findAnimation("playerRightJumpUp");
		else if (_dir == -1) _ani = KEYANIMANAGER->findAnimation("playerLeftJumpUp");
		_ani->start();
		break;
	case PLAYER_RIGHT_JUMP:
		_isLand = false;
		_isJump = true;
		_image = IMAGEMANAGER->findImage("playerJump");
		_ani = KEYANIMANAGER->findAnimation("playerRightJumpUp");
		_ani->start();
		break;
	case PLAYER_LEFT_JUMP:
		_isLand = false;
		_isJump = true;
		_image = IMAGEMANAGER->findImage("playerJump");
		_ani = KEYANIMANAGER->findAnimation("playerLeftJumpUp");
		_ani->start();
		break;
	case PLAYER_RIGHT_MOVE:
		_image = IMAGEMANAGER->findImage("playerWalk");
		_ani = KEYANIMANAGER->findAnimation("playerRightMove");
		_ani->start();
		break;
	case PLAYER_LEFT_MOVE:
		_image = IMAGEMANAGER->findImage("playerWalk");
		_ani = KEYANIMANAGER->findAnimation("playerLeftMove");
		_ani->start();
		break;
	case PLAYER_UP_CLIMB:
		_image = IMAGEMANAGER->findImage("playerClimb");
		_ani = KEYANIMANAGER->findAnimation("playerClimb");
		_ani->start();
		break;
	case PLAYER_DOWN_CLIMB:
		_image = IMAGEMANAGER->findImage("playerClimb");
		_ani = KEYANIMANAGER->findAnimation("playerClimb");
		_ani->start();
		break;
	case PLAYER_EDGE_CLIMB:
		_image = IMAGEMANAGER->findImage("playerClimb");
		_ani = KEYANIMANAGER->findAnimation("playerEdgeClimb");
		_ani->start();
		break;
	case PLAYER_RIGHT_ATTACK:
		_image = IMAGEMANAGER->findImage("playerAtk");
		_ani = KEYANIMANAGER->findAnimation("playerRightAttack");
		_ani->start();
		//SOUNDMANAGER->play("공격", 1.0f);
		break;
	case PLAYER_RIGHT_JUMP_ATTACK:
		break;
	case PLAYER_LEFT_ATTACK:
		_image = IMAGEMANAGER->findImage("playerAtk");
		_ani = KEYANIMANAGER->findAnimation("playerLeftAttack");
		_ani->start();
		//SOUNDMANAGER->play("공격", 1.0f);
		break;
	case PLAYER_LEFT_JUMP_ATTACK:
		break;
	case PLAYER_DOWN_ATTACK:
		_image = IMAGEMANAGER->findImage("playerDownAtk");
		if (_dir == 1) _ani = KEYANIMANAGER->findAnimation("playerRightDownAttack");
		else if (_dir == -1) _ani = KEYANIMANAGER->findAnimation("playerLeftDownAttack");
		_ani->start();
		break;
	case PLAYER_RIGHT_DOWN_ATTACK:
		_image = IMAGEMANAGER->findImage("playerDownAtk");
		_ani = KEYANIMANAGER->findAnimation("playerRightDownAttack");
		_ani->start();
		break;
	case PLAYER_LEFT_DOWN_ATTACK:
		_image = IMAGEMANAGER->findImage("playerDownAtk");
		_ani = KEYANIMANAGER->findAnimation("playerLeftDownAttack");
		_ani->start();
		break;
	case PLAYER_RIGHT_HITTED:
		_image = IMAGEMANAGER->findImage("playerHitted");
		_ani = KEYANIMANAGER->findAnimation("playerRightHitted");
		_ani->start();
		break;
	case PLAYER_LEFT_HITTED:
		_image = IMAGEMANAGER->findImage("playerHitted");
		_ani = KEYANIMANAGER->findAnimation("playerLeftHitted");
		_ani->start();
		break;
	case PLAYER_DEAD:
		break;

	}

}
void player::getColMessage(LPCOLLISION_INFO message)
{
 	if (message != NULL)
	{
		objects* temp;
		RECT _tempRC;
		if(message->_isPlayer)
		{ 
		switch (message->_colType)
		{
			case COL_MONSTER:
			switch (message->index_detail)
			{
			case 0: // 딱정벌레
				if(_immune == false) _currentHP -= 1;
				 _immune = true;
				collisonHitted();
				break;
			case 1: // 해골
				if (_immune == false) _currentHP -= 1;
				_immune = true;
				collisonHitted();
				break;
			case 2:
				break;
			case 3:
				break;
			}
			break;
		case COL_OBJECT:
			switch (message->index_detail)
			{
			case 11: // gem
				static_cast<objects*>(message->object);
				temp = static_cast<objects*>(message->object);

				switch (temp->getGemType())
				{
				case 120:
					_playerGold += 5;
					break;
				case 121:
					_playerGold += 10;
					break;
				case 122:
					_playerGold += 20;
					break;
				case 123:
					_playerGold += 50;
					break;
				case 124:
					_playerGold += 100;
					break;
				case 125:
					_playerGold += 200;
					break;
				}
				break;
			case 12: // 흙
			{
				static_cast<objects*>(message->object);
				temp = static_cast<objects*>(message->object);

				if (IntersectRect(&_tempRC, temp->getRc(), &_playerRC))
				{
					//setPlayerCondition();

					float _width = _tempRC.right - _tempRC.left;
					float _height = _tempRC.bottom - _tempRC.top;
					float _tempWidth = (temp->getRc()->right - temp->getRc()->left) / 2;
					float _templeft = temp->getRc()->left;
					float _tempright = temp->getRc()->right;
					if (_width > _height)
					{
						/*float _pwidth = _playerRC.right - _playerRC.left;
						if (_tempRC.top == temp->getRc().top)
						{
							_offPicxel = true;
							_isLand = true;
							//_isJump = false;
							setIsJump(false);
						}
						if (_isLand)
						{
							//OffsetRect(&_playerRC, 0, _height);
							//_playerRC = RectMakeCenter(_x, _y, 150, 160);
							_y = temp->getRc().top - (_playerRC.bottom - _playerRC.top) / 2 + 5;
							//_y += _jumpPower + 1;
						}
						if (_playerRC.right <= _templeft + _tempWidth || _playerRC.left >= _tempright - _tempWidth
							|| _playerRC.right <= _templeft + _tempWidth && _isJump == true || _playerRC.left >= _tempright - _tempWidth && _isJump == true)
						{
							_isLand = false;
							_offPicxel = false;
							//_isJump = true;
							setIsJump(true);
						}
						if (_tempRC.bottom == temp->getRc().bottom)
						{
						_y = temp->getRc().bottom - (_playerRC.top - _playerRC.bottom) / 2 + 25;
						}*/

						if (_tempRC.bottom == _playerRC.bottom && _tempRC.top == temp->getRc()->top && !_objectLanding)
						{
							_landingObjectSpeedY = temp->getSpeedY();
							_landingObjectSpeedX = temp->getSpeedX();
							_landingObject = temp->getRc();
							_objectLanding = true;
							_offPicxel = true;
							_isLand = true;
							setIsJump(false);
							_y -= _height;
						}
					}

					if (_height > _width)
					{
						if (_tempRC.left == temp->getRc()->left)
						{
							//OffsetRect(&_playerRC, -_width, 0);
							_x = temp->getRc()->left - (_playerRC.right - _playerRC.left) / 2;
						}
						else
						{
							//OffsetRect(&_playerRC, _width, 0);
							_x = temp->getRc()->right + (_playerRC.right - _playerRC.left) / 2;
						}
					}

				}
			}
			break;
			case 13: //포션
				_currentMP += 6;
				if (_currentMP > _maxMP) _currentMP = _maxMP;
				break;
			case 14: //음식
				_currentHP = _maxHP;
				break;
			case 15: // MEAL (체력최대치올리는음식)
				_maxHP += 2;
				_currentHP = _maxHP;
				break;
			case 16: //방울
				break;
			case 17: //음악

				break;
			case 18: //발판
			{
				static_cast<objects*>(message->object);
				temp = static_cast<objects*>(message->object);
				
				if (IntersectRect(&_tempRC, temp->getRc(), &_playerRC))
				{
					//setPlayerCondition();

					float _width = _tempRC.right - _tempRC.left;
					float _height = _tempRC.bottom - _tempRC.top;
					float _tempWidth = (temp->getRc()->right - temp->getRc()->left) / 2;
					float _templeft = temp->getRc()->left;
					float _tempright = temp->getRc()->right;
					if (_width > _height)
					{
						/*float _pwidth = _playerRC.right - _playerRC.left;
						if (_tempRC.top == temp->getRc().top)
						{
							_offPicxel = true;
							_isLand = true;
							//_isJump = false;
							setIsJump(false);
							if (!temp->getDirection() && temp->getisRight()) 
							{
								_x += 1;
							}
							else if (!temp->getDirection() && !temp->getisRight())
							{
								_x -= 1;
							}
						}
						if (_isLand)
						{
							_y = temp->getRc().top - (_playerRC.bottom - _playerRC.top) / 2 + 1;
							//OffsetRect(&_playerRC, 0, -_height);
							//_y += _jumpPower;
						}
						if (_playerRC.right <= _templeft + _tempWidth || _playerRC.left >= _tempright - _tempWidth
							|| _playerRC.right <= _templeft + _tempWidth && _isJump == true || _playerRC.left >= _tempright - _tempWidth && _isJump == true)
						{
							_isLand = false;
							_offPicxel = false;
							//_isJump = true;
							setIsJump(true);
						}
						if (_tempRC.bottom == temp->getRc().bottom)
						{
							_y = temp->getRc().bottom - (_playerRC.top - _playerRC.bottom) / 2 + 25;
						}*/

						if (_tempRC.top == temp->getRc()->top && !_objectLanding)
						{
							_landingObjectSpeedY = temp->getSpeedY();
							_landingObjectSpeedX = temp->getSpeedX();
							_landingObject = temp->getRc();
							_objectLanding = true;
							_offPicxel = true;
							_isLand = true;
							setIsJump(false);
							_y -= _height;

							if (!temp->getDirection() && temp->getisRight())
							{
								_x += 1;
							}
							else if (!temp->getDirection() && !temp->getisRight())
							{
								_x -= 1;
							}
						}
						else if (_tempRC.bottom == _playerRC.bottom && _tempRC.top == temp->getRc()->top && !_objectLanding)
						{
							
						}
					}

					if (_height > _width)
					{
						if (_tempRC.left == temp->getRc()->left)
						{
							//OffsetRect(&_playerRC, -_width, 0);
							_x = temp->getRc()->left - (_playerRC.right - _playerRC.left) /2;
						}
						else
						{
							//OffsetRect(&_playerRC, _width, 0);
							_x = temp->getRc()->right + (_playerRC.right - _playerRC.left) / 2;
						}
					}

				}
			}
			break;
			case 19: //접시
				break;
			case 20: //가짜 벽
			{
				static_cast<objects*>(message->object);
				temp = static_cast<objects*>(message->object);

				if (IntersectRect(&_tempRC, temp->getRc(), &_playerRC))
				{
					//setPlayerCondition();

					float _width = _tempRC.right - _tempRC.left;
					float _height = _tempRC.bottom - _tempRC.top;
					float _tempWidth = (temp->getRc()->right - temp->getRc()->left) / 2;
					float _templeft = temp->getRc()->left;
					float _tempright = temp->getRc()->right;

					if (_height > _width)
					{
						if (_tempRC.left == temp->getRc()->left)
						{
							OffsetRect(&_playerRC, -_width, 0);
							//_x = temp->getRc()->left - (_playerRC.right - _playerRC.left) / 2;
						}
						else
						{
							OffsetRect(&_playerRC, _width, 0);
							//_x = temp->getRc()->right + (_playerRC.right - _playerRC.left) / 2;
						}
					}

				}
			}
				break;
			case 21: //광맥
				break;
			}break;

				case COL_NPC:
				{
					switch (message->index_detail)
					{
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					}
					break;
				}
				break;
			}
		}

 		if(!message->_isPlayer && _canAtk)
		{
			switch (message->_colType)
			{
			case COL_MONSTER:
				switch (message->index_detail)
				{
				case 0: // 딱정벌레
					collisonAttack();
					break;
				case 1: // 해골
					collisonAttack();
					break;
				}
			case COL_OBJECT:
				switch (message->index_detail)
				{
				case 12:
					collisonAttack();
					break;
				case 16:
					collisonAttack();
					break;
				case 21:
					//SOUNDMANAGER->play("광맥");
					switch (_playerMainCondition)
					{
					case 10:
						_canAtk = false;
						break;

					case 12:
						_canAtk = false;
						break;
					case 14:
						collisonAttack();
						break;
					case 15:
						collisonAttack();
						break;
					case 16:
						collisonAttack();
						break;
					}
							
					break;
				}
			break;
			}
		}
			

	}SAFE_DELETE(message);
}
