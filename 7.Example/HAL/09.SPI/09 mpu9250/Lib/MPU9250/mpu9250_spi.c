#include "mpu9250_spi.h"

MPU      mpu_value;
uint16_t BUF[6];
uint8_t  x_axis, y_axis, z_axis;

void delay_us(uint32_t t) {
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000000);
    HAL_Delay(t - 1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
}
void delay_ms(uint32_t t) { HAL_Delay(t); }

static uint8_t SPI1_ReadWriteByte(uint8_t data) {
    HAL_SPI_Transmit(&hspi3, &data, 1, 0xFF);
    uint8_t buff;
    HAL_SPI_Receive(&hspi3, &buff, 1, 0xFF);
    return buff;
}
/***************************************************************/
// SPI¡¦???
// reg: addr
// value:????
/***************************************************************/
uint8_t MPU9250_Write_Reg(uint8_t reg, uint8_t value) {
    uint8_t status;
    MPU_9250_ENABLE;                   //	MPU9250_CS=0;  //????MPU9250
    status = SPI1_ReadWriteByte(reg);  //¡¦???reg???¡¦
    SPI1_ReadWriteByte(value);         //¡¦???????
    MPU_9250_DISENABLE;                //	MPU9250_CS=1;  //?¡ì??MPU9250
    return (status);                   //
}
//---------------------------------------------------------------//
// SPI????
// reg: addr
uint8_t MPU9250_Read_Reg(uint8_t reg) {
    uint8_t reg_val;
    MPU_9250_ENABLE;                     //	MPU9250_CS=0;  //????MPU9250
    SPI1_ReadWriteByte(reg | 0x80);      // reg???¡¦+???¨¹??
    reg_val = SPI1_ReadWriteByte(0xff);  //????????
    MPU_9250_DISENABLE;                  //	MPU9250_CS=1;  //?¡ì??MPU9250
    return (reg_val);
}
/***************************************************************/
// MPU????i2c ????
// I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
// I2C_SLVx_REG:   reg
// I2C_SLVx_Data out:  value
/***************************************************************/
static void i2c_Mag_write(uint8_t reg, uint8_t value) {
    MPU9250_Write_Reg(I2C_SLV0_ADDR, MPU9250_AK8963_ADDR);  //?????????????¡¦,mode: write
    delay_us(200);
    MPU9250_Write_Reg(I2C_SLV0_REG, reg);  // set reg addr
    delay_us(200);
    MPU9250_Write_Reg(I2C_SLV0_DO, value);  // send value
    delay_us(200);                          //?????¨¾??MPU????I2C????????????????????????????????
}
/***************************************************************/
// MPU????i2c ????
// I2C_SLVx_ADDR:  MPU9250_AK8963_ADDR
// I2C_SLVx_REG:   reg
// return value:   EXT_SENS_DATA_00 register value
/***************************************************************/
static uint8_t i2c_Mag_read(uint8_t reg) {
    uint8_t a;
    MPU9250_Write_Reg(I2C_SLV0_ADDR, MPU9250_AK8963_ADDR | 0x80);  //?????????????¡¦??mode??read
    delay_us(200);
    MPU9250_Write_Reg(I2C_SLV0_REG, reg);  // set reg addr
    delay_us(200);
    MPU9250_Write_Reg(I2C_SLV0_DO, 0xff);  // read
    delay_us(200);                         //?????¨¾??MPU????I2C??????????????????????????????????????
    return MPU9250_Read_Reg(EXT_SENS_DATA_00);
}

//****************??????MPU9250????????????????pdf????????************************
void Init_MPU9250(void) {
    MPU9250_Write_Reg(PWR_MGMT_1, 0x80);  //????????¡Á???
    delay_ms(100);                        //???????¡è????

    /**********************Init SLV0 i2c**********************************/
    // Use SPI-bus read slave0
    MPU9250_Write_Reg(INT_PIN_CFG, 0x32);         // INT Pin / Bypass Enable Configuration  ??????????
    MPU9250_Write_Reg(I2C_MST_CTRL, 0x4d);        // I2C MAster mode and Speed 400 kHz
    MPU9250_Write_Reg(USER_CTRL, 0x20);           // I2C_MST _EN
    MPU9250_Write_Reg(I2C_MST_DELAY_CTRL, 0x01);  //????????I2C_SLV0 _DLY_ enable
    MPU9250_Write_Reg(I2C_SLV0_CTRL, 0x81);       // enable IIC	and EXT_SENS_DATA==1 Byte

    ///*******************Init GYRO and ACCEL******************************/
    MPU9250_Write_Reg(CONFIG, 0x07);          //???¡§???¡§??????????????0x07(3600Hz)???????¡Â?????¡§Internal_Sample_Rate==8K
    MPU9250_Write_Reg(SMPLRT_DIV, 0x07);      //??????????????????????0x07(1kHz) (SAMPLE_RATE= Internal_Sample_Rate / (1 + SMPLRT_DIV) )
    MPU9250_Write_Reg(GYRO_CONFIG, 0x18);     //??????¡Á??¨§?¡ã????¡¦??¡ì??????????0x18(??¡Á??¨§??2000deg/s)
    MPU9250_Write_Reg(ACCEL_CONFIG_2, 0x08);  //?????????¡§???¡§???? ?????? ??0x08  ?¡§1.13kHz??
    MPU9250_Write_Reg(ACCEL_CONFIG, 0x10);    //??????¡Á??¨§??????¡¦??¡ì?¡ã???¡§???¡§??????????????0x00/+-2g. 0x08/+-4g. 0x10/+-8g. 0x18(??¡Á??¨§??16G)

    /**********************Init MAG **********************************/
    i2c_Mag_write(AK8963_CNTL2_REG, AK8963_CNTL2_SRST);  // Reset AK8963
    i2c_Mag_write(AK8963_CNTL1_REG, 0x12);               // use i2c to set AK8963 working on Continuous measurement mode1 & 16-bit output

    //  MPU9250_Write_Reg(MPU9250_RA_FIFO_EN, 0xFF);//FIFO??
    //  delay_ms(10);

    //  /***********************DMP??**********************************/
    //  MPU9250_Write_Reg(MPU9250_RA_INT_PIN_CFG,0x00); //??????
    //  MPU9250_Write_Reg(MPU9250_RA_INT_ENABLE,0x01);  //FIFO????

    delay_ms(10);
}

//************************??????????**************************/
void READ_MPU9250_ACCEL(void)  //
{
    BUF[0]             = MPU9250_Read_Reg(ACCEL_XOUT_L);
    BUF[1]             = MPU9250_Read_Reg(ACCEL_XOUT_H);
    mpu_value.Accel[0] = (BUF[1] << 8) | BUF[0];
    mpu_value.Accel[0] /= 164;  //????????X?¨¢????
    BUF[2]             = MPU9250_Read_Reg(ACCEL_YOUT_L);
    BUF[3]             = MPU9250_Read_Reg(ACCEL_YOUT_H);
    mpu_value.Accel[1] = (BUF[3] << 8) | BUF[2];
    mpu_value.Accel[1] /= 164;  //????????Y?¨¢????
    BUF[4]             = MPU9250_Read_Reg(ACCEL_ZOUT_L);
    BUF[5]             = MPU9250_Read_Reg(ACCEL_ZOUT_H);
    mpu_value.Accel[2] = (BUF[5] << 8) | BUF[4];
    mpu_value.Accel[2] /= 164;  //????????Z?¨¢????
}
/**********************??????????*****************************/
void READ_MPU9250_GYRO(void) {
    BUF[0]            = MPU9250_Read_Reg(GYRO_XOUT_L);
    BUF[1]            = MPU9250_Read_Reg(GYRO_XOUT_H);
    mpu_value.Gyro[0] = (BUF[1] << 8) | BUF[0];
    mpu_value.Gyro[0] /= 164;  //????????X?¨¢????
    BUF[2]            = MPU9250_Read_Reg(GYRO_YOUT_L);
    BUF[3]            = MPU9250_Read_Reg(GYRO_YOUT_H);
    mpu_value.Gyro[1] = (BUF[3] << 8) | BUF[2];
    mpu_value.Gyro[1] /= 164;  //????????Y?¨¢????
    BUF[4]            = MPU9250_Read_Reg(GYRO_ZOUT_L);
    BUF[5]            = MPU9250_Read_Reg(GYRO_ZOUT_H);
    mpu_value.Gyro[2] = (BUF[5] << 8) | BUF[4];
    mpu_value.Gyro[2] /= 164;  //????????Z?¨¢????
}

/**********************??????????***************************/
// i2c_Mag_read(AK8963_ST2_REG) ????????????????
//?????????¨¢???????¡Â??reading this register means data reading end
// AK8963_ST2_REG ????????????¡¦??????????¨§??????
//???¨¦???? MPU9250 PDF
/**********************************************************/
void READ_MPU9250_MAG(void) {
    //	uint8_t x_axis,y_axis,z_axis;

    x_axis = i2c_Mag_read(AK8963_ASAX);  // X?¨¢?¨¦?????¡Â????
    y_axis = i2c_Mag_read(AK8963_ASAY);
    z_axis = i2c_Mag_read(AK8963_ASAZ);

    if ((i2c_Mag_read(AK8963_ST1_REG) & AK8963_ST1_DOR) == 0)  // data ready
    {
        //????????X?¨¢????
        BUF[0] = i2c_Mag_read(MAG_XOUT_L);                          // Low data
        if ((i2c_Mag_read(AK8963_ST2_REG) & AK8963_ST2_HOFL) == 1)  // data reading end register & check Magnetic sensor overflow occurred
        {
            BUF[0] = i2c_Mag_read(MAG_XOUT_L);  // reload data
        }
        BUF[1] = i2c_Mag_read(MAG_XOUT_H);                          // High data
        if ((i2c_Mag_read(AK8963_ST2_REG) & AK8963_ST2_HOFL) == 1)  // data reading end register
        {
            BUF[1] = i2c_Mag_read(MAG_XOUT_H);
        }
        mpu_value.Mag[0] = ((BUF[1] << 8) | BUF[0]) * (((x_axis - 128) >> 8) + 1);  //?¨¦???????? ??????/RM-MPU-9250A-00 PDF/ 5.13

        //????????Y?¨¢????
        BUF[2] = i2c_Mag_read(MAG_YOUT_L);                          // Low data
        if ((i2c_Mag_read(AK8963_ST2_REG) & AK8963_ST2_HOFL) == 1)  // data reading end register
        {
            BUF[2] = i2c_Mag_read(MAG_YOUT_L);
        }
        BUF[3] = i2c_Mag_read(MAG_YOUT_H);                          // High data
        if ((i2c_Mag_read(AK8963_ST2_REG) & AK8963_ST2_HOFL) == 1)  // data reading end register
        {
            BUF[3] = i2c_Mag_read(MAG_YOUT_H);
        }
        mpu_value.Mag[1] = ((BUF[3] << 8) | BUF[2]) * (((y_axis - 128) >> 8) + 1);

        //????????Z?¨¢????
        BUF[4] = i2c_Mag_read(MAG_ZOUT_L);                          // Low data
        if ((i2c_Mag_read(AK8963_ST2_REG) & AK8963_ST2_HOFL) == 1)  // data reading end register
        {
            BUF[4] = i2c_Mag_read(MAG_ZOUT_L);
        }
        BUF[5] = i2c_Mag_read(MAG_ZOUT_H);                          // High data
        if ((i2c_Mag_read(AK8963_ST2_REG) & AK8963_ST2_HOFL) == 1)  // data reading end register
        {
            BUF[5] = i2c_Mag_read(MAG_ZOUT_H);
        }
        mpu_value.Mag[2] = ((BUF[5] << 8) | BUF[4]) * (((z_axis - 128) >> 8) + 1);
    }
}
