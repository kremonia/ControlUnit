
#pragma once

struct Point32_t
{
	int32_t x;
	int32_t y;
	int32_t z;
};

union union64
{
	uint64_t u64;
	int64_t  i64;
	uint32_t u32[2];
	int32_t  i32[2];
	uint16_t u16[4];
	int16_t  i16[4];
	uint8_t u8[8];
	int8_t  i8[8];
};

union union32
{
	uint32_t u32;
	int32_t  i32;
	uint16_t u16[2];
	int16_t  i16[2];
	uint8_t u8[4];
	int8_t  i8[4];
};

union union16
{
	uint16_t u16;
	int16_t  i16;
	uint8_t u8[2];
	int8_t  i8[2];
};

enum CHANNEL { CHANNEL_X = 0, CHANNEL_Y = 1, CHANNEL_Z = 2, CHANNEL_U = 3, CHANNEL_X2 = 4, CHANNEL_Y2 = 5, CHANNEL_Z2 = 6, CHANNEL_U2 = 7 };

enum CameraWorkMode {
	Continious,
	SofwareTrigger,
	HardwareTrigger
};

enum CameraParam {
	camera_pixel_x = 0,
	camera_pixel_y = 1,
	camera_x_nm = 2,
	camera_y_nm = 3,
	camera_FPS = 4,
	camera_gain = 5,
	camera_exp = 6,
	camera_flip = 7, // 2bit color 1bit flipY 0bit flipX
};

struct focus_param_t
{
	int16_t axis;
	int16_t status;
	int32_t roi;
	int32_t porog1;
	int32_t porog2;
	int32_t shag_max;
	int32_t shag;
	int32_t focus_min;
	int32_t z;
	int32_t focus;
	int32_t focus_dir;
	int8_t  cnt;
	int8_t  ind;
	int16_t delay;
};

enum micro_command
{
	micro_FUNCTION=0,
	micro_VERSIA=1,
	micro_SERIAL=2,
	micro_GET_PARAM=3,
	micro_SET_PARAM=4,
	micro_OPEN=5,
	micro_CLOSE=6,
	micro_GET_IMAGE=7,
	micro_ECHO=8,
	micro_STATUS=9,
	micro_OBJ_GET=10,
	micro_OBJ_NEXT=11,
	micro_OBJ_PREV=12,
	micro_HOME=13,
	micro_STOP_MOTOR=14,
	micro_ABS=15,
	micro_REL=16,
	micro_LIGHT_MINUS=17,
	micro_LIGHT_PLUS = 18,
	micro_LIGHT_STATUS = 19,
	micro_STEP=20,
	micro_VEL=21,
	micro_ENC=22,
	micro_LED_ON=23,
	micro_LED_OFF=24,
	micro_LED_TIME=25,
	micro_SCAN=26,
	micro_MARKER=27,

	micro_READ=28,
	micro_WRITE=29,

	micro_MOVE = 30,

	micro_EE_READ=32,
	micro_EE_WRITE=33,

	micro_ABS_POL = 34,
	micro_REL_POL = 35,

	micro_LIGHT_GET=36,
	micro_LIGHT_SET=37,

	micro_Vacuum = 38,
	micro_STOP = 39,
	micro_START = 40,

	micro_CENTR = 41,
	micro_LOAD = 42,
	micro_FOCUS = 43,
	micro_CORR = 44,

	micro_CAMERA_WIDTH = 46,
	micro_CAMERA_HEIGHT = 47,

	micro_CAMERA_FLIP_X = 48,
	micro_CAMERA_FLIP_Y = 49,
	micro_CAMERA_GAIN = 50,
	micro_CAMERA_EXPOSE = 51,
	micro_CAMERA_AUTO_GAIN = 52,
	micro_CAMERA_AUTO_EXPOSE = 53,
	micro_CAMERA_AUTO_WHITE = 54,
	micro_CAMERA_AUTO = 55,
	micro_CAMERA_MODE = 56,
	micro_CAMERA_SATURATION = 59,
	micro_CAMERA_GAMMA = 60,
	micro_CAMERA_BRIGHTNESS = 61,
	micro_CAMERA_RED = 62,
	micro_CAMERA_GREEN = 63,
	micro_CAMERA_BLUE = 64,

	micro_FOCUS_GET_PARAM = 65,
	micro_FOCUS_SET_PARAM = 66,
	micro_FOCUS_CALCULATE =67,
	micro_FOCUS_FIND = 68,

	micro_POL_ANGLE = 69,
	micro_POL_DELTA = 70,
	micro_POL_SET_SPEED = 71,
	micro_POL_HOME = 72,
	micro_POL_STOP = 73,
	micro_POL_READ_ANGLE = 74,
	micro_POL_READ_STATE = 75,
	micro_POL_MOVE = 76,
	micro_POL_POS_RESET = 77,
	micro_POL_STEP_ABS = 78,
	micro_POL_STEP_REL = 79,

	micro_SVK_SCAN = 80,

	micro_DeskDllOpen = 81,
	micro_DeskDllClose = 82,
	micro_DeskDllSetup = 83,
	micro_DeskDllHome = 84,
	micro_DeskDllSerialGet = 85,
	micro_DeskDllTimeoutSet = 86,
	micro_DeskDllXYZMoveAbs = 87,
	micro_DeskDllXYZMoveRel = 88,
	micro_DeskDllAxisMoveAbs = 89,
	micro_DeskDllAxisMoveRel = 90,
	micro_DeskDllPolMoveAbs = 91,
	micro_DeskDllPolMoveRel = 92,
	micro_DeskDllStatus = 93,
	micro_DeskDllLightGet = 94,
	micro_DeskDllLightSet = 95,
	micro_DeskDllObjectiveGet = 96,
	micro_DeskDllObjectiveSet = 97,
	micro_DeskDllBFDFGet = 98,
	micro_DeskDllBFDFSet = 99,
	micro_DeskDllApertureGet = 100,
	micro_DeskDllApertureSet = 101,
	micro_DeskDllPortUpGet = 102,
	micro_DeskDllPortUpSet = 103,
	micro_DeskDllPortDownGet = 104,
	micro_DeskDllPortDownSet = 105,
	micro_DeskDllFiltrGet = 106,
	micro_DeskDllFiltrSet = 107,

	micro_DeskUp = 108,
	micro_DeskDown = 109,
	micro_DeskSvet =110,
	micro_DeskIk =111,
	micro_PowerOn =112,
	micro_PowerOff = 113,

	camdesk_load_DLL = 114,

	micro_noWaite = 0x80 // не ждем окончания операции, завершение фиксируем по обнулению текущей команды в статусе
};

struct camera_status_t
{
	bool isNewImage=false;
	bool isWork = false;
	bool isStop = false;
	bool isFocus = false;
};

struct pak_32
{
	uint8_t x55;  
	uint8_t dlina;  
	uint8_t dest; 
	uint8_t ist;  
	uint8_t key;  
	int8_t dlina_inf;
	uint16_t kz_adr;
	int32_t data[13]; //  ������  
	uint16_t ksum;
};

struct pak_8
{
	uint8_t x55;
	uint8_t dlina;
	uint8_t dest;
	uint8_t ist;
	uint8_t key;
	int8_t dlina_inf;
	uint16_t kz_adr;
	uint8_t data[4*13];
	uint16_t ksum;
};

struct pak_0
{
	uint8_t buf[sizeof(pak_32)];
};


union paket // �������� 64 �����
{
	pak_0   pak0;
	pak_8   pak8;
	pak_32  pak32;
};

struct motor_param_t
{
	uint8_t tip;
	uint8_t csPin;
	uint8_t enPin;
	uint8_t current;

	uint8_t ihold;
	uint8_t step_error;
	uint16_t accelerate_max;

	uint32_t timeout;
	uint32_t vel_max;
	uint32_t oborot_value;
	uint32_t oborot_step;
	uint32_t oborot_enc;

	int32_t centr=100000000;
	int32_t load =1000000;
	int32_t nm_max = 0;
};

struct motor_status_t
{
	int32_t vel_act=0;
	int32_t step_dest=0;
	int32_t step_act=0;
	int32_t enc_act=0;
	uint8_t status=0;
	uint8_t mode=0;
	uint8_t sensor=0;
	uint8_t vel_proc=0;
};

struct micro_param_t
{
	uint8_t dev_id;
	uint8_t objective_enable;
	uint8_t motor_enable;
	uint8_t joy_enable;

	uint8_t joy_reverse;
	uint8_t joy_group;
	uint8_t joy_speed_obj[6];

	uint8_t baza_enable;
	uint8_t corr_enable;
	uint8_t keyboard_enable;
	uint8_t rezerv1;

	uint16_t marker_impuls;
	uint16_t rezerv2;

	int32_t corr_z[2][2];
};

struct work_status_t
{
	uint8_t isStop=0;
	uint8_t commanda=0;
	uint8_t isNewData=0;
	uint8_t rezerv=0;
};

enum io_bit
{
	bit_group,
	bit_in1,
	bit_in2,
	bit_in3,
	bit_in4_vacuum,
	bit_DF,
	bit_portUp1,
	bit_portUp2,
	bit_portDown,
	bit_filtr1,
	bit_filtr2,
	bit_Mirror
};

struct micro_status_t
{
//	uint32_t nomer = 0;

	uint8_t commanda=0;
	uint8_t obj = 255;
	uint8_t motor_move=0;
	uint8_t is_Stop = 0;

	uint8_t motor_base = 0;
	uint8_t light_status = 0;
	uint8_t light_value = 0;
	uint8_t commanda_new = 0;

	uint16_t key=0;
	uint16_t io=0;

	uint32_t motor_nm[8] = { 0 };

//	int32_t polar_ugol=0;
//	int32_t polar_enc=0;

	uint32_t paket_count=0;
	uint32_t rs485_error=0;

};

enum micro_device_id
{
	dev_id_0 = 0,
	dev_id_Camera1 = 1,
	dev_id_Camera2 = 2,
	dev_id_CameraDesk = 3,
	dev_id_Camera2Desk = 4,
	dev_id_Integral = 5,
	dev_id_GUF = 6,
	dev_id_Him = 7,
	dev_id_Foton = 8,
	dev_id_SVK = 9,
	dev_id_Scan = 10
};

enum rs485_device_id
{
	ID_OBJ_MCU = 1, 		// ���������� ���������� �����������
	ID_CONTROL_PANEL = 2,	// ����� ����������
	ID_CONTROL_BLOCK = 3,	// ���� ���������� ����������� (������)
	ID_BROADCAST = 4,		// ����������������� �������
	ID_POLAROID = 5,		// ��������
};


enum rs485_status
{
	RS_READ = 1,
	RS_WRITE = 2
};

enum rs485_command_polaroid
{
	CMD_TURN_ANGLE	=1,
	CMD_TURN_DELTA = 2,
	CMD_SET_SPEED = 3,
	CMD_HOME = 4,
	CMD_STOP = 5,
	CMD_READ_ANGLE = 6,
	CMD_READ_STATE = 7,
	CMD_MOVE = 8,
	CMD_POS_RESET = 9,
	CMD_SET_XTARGET = 10,
};

enum rs485_command
{
	CMD_SWITCH_OBJ = 1, //-������������ ���������
	CMD_MOVE_MIRROR = 2, //-������������ �������
	CMD_MOVE_DIAPHRAGM = 3, //-������������ ���������
	CMD_CHANGE_LIGHTER_U = 4, //-��������� ������� ����������
	CMD_CHANGE_CONFIG_LIGHTER = 5, //-��������� ������� ����������(� ������� � ������)
	CMD_CHANGE_CONFIG_DIAPH = 6, //-��������� ��������� ���������
	CMD_SAVE_CONFIG_PARAMS = 7, //-���������� ���������������� ����������
	CMD_SET_ACTIVE_OBJS = 8, //-��������� ����������
	CMD_CLEAR_ACTIVE_OBJS = 9, //-����� �������� ����������
	CMD_INIT_MCU_CONFIG = 10, //-������ �������� �� ������
	CMD_TEST_COUNT = 11,
	CMD_CONFIG_SENS = 12, //-����� ��������� ������������ �������

	CMD_SWITCH_LIGHTER = 13, //-������������ ��������� ����������
	CMD_SWITCH_OPFILTER = 14, //-������������ �������������
	CMD_SWITCH_UPPORT = 15, //-������������ �������� �����
	CMD_SWITCH_LOWPORT = 16, //-������������ ������� �����
	CMD_INIT_DEVICES = 17, //-������������� ��������� �� - ��

	CMD_EMPTY = 255 //- ��� �������
};

enum rs485_data
{
	RS_DEV_ID = 0,				//ID ����������
	RS_CURRENT_OBJ = 1,			//������� ��������
	RS_DIR_OBJ = 2,				//����������� ������������ ���������: 1 - ������, 2 - �����
	RS_COMMAND = 3,				//�������� ������� (��. ������ ������)
	RS_DEVICES = 4,				// ����� �� �� ������������
	RS_MIRROR_POS = 5,			//��������� ������� (1 � ������� ��������, 2 � ������� �������)
	RS_DIAPH_POS = 6,			//��������� ��������� (1 � ������� �����, 2 � �������� �����)
	RS_SEC_COMMAND = 7,			//�������������� ������� (����������) 
	RS_STOP_DIAPH = 8,			// ����� �� �� ������������

	RS_LIGHTER_I = 9,			//��� �� ����������
	RS_LIGHTER_U = 10,			//���������� �� ����������

	RS_FUN_LEDS = 11,				// ������ ��� ���������� ���������� �������
	RS_ADC_OUT_I = 12,				// (��. ������� �� �������� �������)
	RS_ADC_OUT_U = 13,				//
	RS_ADC_IN_U = 14,				//
	RS_LIFETIME = 15,				//
	RS_STATUS_DELTA_ENC_U = 16,		//
	RS_DELTA_ENC_I_TYPE_LIGHTER = 17,		//
	RS_ADC_SCALE_U = 18,			//
	RS_ADC_SCALE_I = 19,				//
	RS_ADC_SHIFT_U = 20,			//
	RS_ADC_SHIFT_I = 21,				//
	RS_DAC_SCALE_U = 22,			//
	RS_DAC_SHIFT_U = 23,				//
	RS_FLAGS = 24,				//

	LIFE_TIME = 25,			//��� ������ ������� ������. �� �����������

	STATUS_SYSTEM = 26,		//������ �������
	STATUS_FUNCTION = 27,		//������ ���������� �������
	SENS_DATA = 28,			//������ � ��������
	LIGHTER_DELTA = 29,			//�������� ��������� ������� ����������

	IS_END_INIT = 30,			//
	TEST_COUNT = 31,			//
	LIGHTER_SHIFT = 32,			//
	SAVE_CONFIG = 33,			//
	CONFIG_MODE = 34,			//
	ACTIVE_OBJS = 35,			//

	RS_USER1 = 36,			// ������ ��� ���������� ���������� �������
	RS_USER2 = 37,			// (��. ������� �� �������� �������)
	RS_USER3 = 38,			//
	RS_USER4 = 39,			//
	RS_USER5 = 40,			//
	RS_USER6 = 41,			//
	RS_USER7 = 42,			//
	RS_USER8 = 43,			//
	RS_USER9 = 44,			//
	RS_USER10 = 45,			//
	RS_USER11_0 = 46,			//
	RS_USER11_1 = 47,			//
	RS_USER12_0 = 48,			//
	RS_USER12_1 = 49,			//
	RS_USER13_0 = 50,			//
	RS_USER13_1 = 51,			//
	RS_USER14_0 = 52,			//
	RS_USER14_1 = 53,			//
	RS_NO_DATA = 54,				//

	DF_DISABLED = 55,			//���� ���������� ������ ������� ����
	DF_ENGAGED = 56,			//���� ���� ��������� � ������ ������� ����

	LIGHTER_STATE = 57,			//��������� ���������� 	(1 - Halogen(��������), 2 - LED(DF), 3 - LED(����������))

	OPFILTER_BLOCK_STATE = 58,	//��������� ����� �������������	(1 - ��� �������, 2 - �����, 3 - �������)

	UPPER_PORT_STATE = 59,		//��������� �������� ����� 	(1 - ��� �������, 2 - ������ �������, 3 - ��������������)

	LOWER_PORT_STATE = 60		//��������� ������� ����� (1 - �����, 2 - ������)
};
