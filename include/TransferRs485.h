#pragma once
#include "arduino.h"

#define RS485_PIN_RXTX		34

#define RS485_PACKAGE_SIZE      (10)	//8
#define RS485_MAX_LENGTH        (32)

#define START_PACKAGE			(0x23)
#define END_PACKAGE 			(0x0D)

#define RS_TX_DELAY				(1000)  // 1000 MS

#define SLAVE_ID				(ID_OBJ_MCU)

#define RS485_UART						//&huart3
#define set_receive_params(buf, size) 	;//(HAL_UART_Receive_DMA(RS485_UART, (buf), (size)))
#define check_receive_status()			0//(hdma_usart3_rx.State == HAL_DMA_STATE_READY)
#define writes(buf, size)					({\
	digitalWrite(RS485_PIN_RXTX, HIGH); \
	Serial2.write(buf, size); \
	digitalWrite(RS485_PIN_RXTX, LOW); \
})

#define RS485_SPEED				(115200)

#define RS_GROUPS			4
#define RS_REGISTERS		16


struct package_info {
	uint8_t x23;
	uint8_t dev_id;
	uint8_t status;
	uint8_t cmd;
	uint8_t key;
	uint8_t data0;
	uint8_t data;
	uint16_t ksum;
};

struct package_info_new {
	uint8_t x23;
	uint8_t dev_id;
	uint8_t cmd;
	uint32_t data32;
	uint16_t ksum;
};

class TransferRs485
{
public:
	TransferRs485();
	~TransferRs485();

	void Setup(void);
	void Loop(void);

	uint32_t time_updateStatusRS = 0;
	uint32_t time_update = 0;
	uint8_t state_read_obj = 0;
	uint8_t isNeedReadObj = 0;
	uint8_t m_adr=0;
	void init();
	bool readPak(void);
	void writePackage(uint8_t* info);


	int dataRead(uint8_t adr, int32_t* p_data, uint16_t timeout=1);
	int dataWrite(uint8_t adr, int32_t* p_data);
	void Obj_mcu();
	void Control_panel(void);
	void Control_blok(void);
	void Polaroid(void);

	uint16_t getData(uint16_t index);
	void setData(uint16_t index, uint16_t value);

	package_info infoLast;
//	uint8_t parse_package();
	uint16_t CrcXmodemUpdata(uint16_t crc, uint8_t data);
	uint16_t calc_crc(uint8_t* data, uint16_t size);

	uint16_t rs_data[RS_GROUPS * RS_REGISTERS];
	uint8_t outputRS[RS485_PACKAGE_SIZE];
	uint8_t inputRS[RS485_PACKAGE_SIZE];
	uint8_t inputLengthRS;
	uint8_t isOutputRS;
	uint8_t is_received;
	uint8_t is_start_receive;
	uint8_t is_transmited;

	package_info info;
	package_info_new info_new;
};
