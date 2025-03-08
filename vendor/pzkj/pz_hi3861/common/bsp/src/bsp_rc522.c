#include "bsp_rc522.h"

#define HI_GPIO_VALUE_LOW HI_GPIO_VALUE0
#define HI_GPIO_VALUE_HIGH HI_GPIO_VALUE1

// **SPI 初始化**
hi_u32 rc522_init(void) {
    hi_u32 ret = HI_ERR_SUCCESS;
    ret |= hi_gpio_init();
    
    // **GPIO 复用配置**
    ret |= hi_io_set_func(RC522_SCK_PIN, RC522_SCK_GPIO_FUN);
    ret |= hi_io_set_func(RC522_MISO_PIN, RC522_MISO_GPIO_FUN);
    ret |= hi_io_set_func(RC522_MOSI_PIN, RC522_MOSI_GPIO_FUN);
    ret |= hi_io_set_func(RC522_CS_PIN, RC522_CS_GPIO_FUN);

    ret |= hi_io_set_func(RC522_RST_PIN, RC522_RST_GPIO_FUN);
    ret |= hi_gpio_set_dir(RC522_RST_PIN, HI_GPIO_DIR_OUT);

    // **配置 SPI 工作模式**
    hi_spi_cfg_init_param spi_init_param = { .is_slave = HI_FALSE };  // 主模式
    hi_spi_cfg_basic_info spi_config = {
        .cpol = HI_SPI_CFG_CLOCK_CPOL_0,  // 时钟极性 0
        .cpha = HI_SPI_CFG_CLOCK_CPHA_0,  // 时钟相位 0
        .fram_mode = HI_SPI_CFG_FRAM_MODE_MOTOROLA,
        .data_width = HI_SPI_CFG_DATA_WIDTH_E_8BIT,  // 8bit 通信
        .endian = HI_SPI_CFG_ENDIAN_LITTLE,  // 小端
        .freq = 1000000    
    };

    ret |= hi_spi_init(RC522_SPI_ID, spi_init_param, &spi_config);  // 初始化 SPI
    // 复位 RC522 开始工作
    ret |= rc522_reset();

    return ret;
}

// **复位 RC522**
hi_u32 rc522_reset(void) {
    hi_u32 ret;
    ret |= hi_gpio_set_ouput_val(RC522_RST_PIN, HI_GPIO_VALUE_LOW);
    hi_udelay(100000); // 100ms
    ret |=  hi_gpio_set_ouput_val(RC522_RST_PIN, HI_GPIO_VALUE_HIGH);
    hi_udelay(100000); // 100ms
    return ret;
}

// *SPI 读寄存器*
hi_u8 rc522_read_register(hi_u8 reg) {
    hi_u8 ret;
    hi_u8 addr = (reg & 0x7E) | 0x80;  // 设置最高、最低位 1，表示读操作
    hi_u8 read_data = 0;

    // **执行 SPI 读写**
    ret = hi_spi_host_writeread(RC522_SPI_ID, &addr, &read_data, 1);

    if(ret != HI_ERR_SUCCESS)
    {
        printf("[DEBUG] 读取失败, ret = &d\n",ret);
        return -1;
    }
    return read_data;
}

// **SPI 写寄存器**
hi_u8 rc522_write_register(hi_u8 reg, hi_u8 value) {
    hi_u8 ret;
    hi_u8 addr = (reg & 0x7E);  // 最高位设 0 表示写操作
    hi_u8 write_data = value;

    ret |=  hi_spi_host_writeread(RC522_SPI_ID, &addr, &write_data, 1);

    return ret;
}

void rc522_write_read_register(hi_u8 addr, hi_u8 writedata, hi_u8 *read_data, hi_u8 read_len, hi_bool is_read)
{
    int ret = 0;
    unsigned char writebuff[2] = {0};
    if (is_read) {
        // 读取模式：发送寄存器地址，读取数据
        writebuff[0] = (addr & 0x7e) | 0x80;  // LSM6DSL 规定：读操作地址最高位为 1
        writebuff[1] = 0x00;  // 读取时第二个字节无意义
    } else {
        // 写入模式：发送寄存器地址 + 写入数据
        writebuff[0] = addr & 0x7e;  // 写操作地址最高位为 0
        writebuff[1] = writedata;
    }

    // 读取前清空接收缓冲区
    if (is_read) {
        memset_s(read_data, read_len + 1, 0x0, read_len);
    }

    ret = hi_spi_host_writeread(RC522_SPI_ID, &writebuff, read_data, read_len);

    if (ret != HI_ERR_SUCCESS) {
        printf("spi %s [%02X] fail! %x\n", is_read ? "read" : "write", addr, ret);
    }
}

// **打开 RC522 天线**
void rc522_antenna_on(void) {
    hi_u8 value = rc522_read_register(0x14);
    if (!(value & 0x03)) {  
        rc522_write_register(0x14, value | 0x03); // 使能 TX1, TX2
    }
}

// **检测是否有 NFC 卡**
hi_u8 rc522_check_card(void) {
    hi_u8 status;
    hi_u8 buffer[2];
    hi_u8 bufferSize = sizeof(buffer);

    status = rc522_request(0x26, buffer, &bufferSize);  // 0x26: Request Mode for Type A
    if (status == HI_ERR_SUCCESS) {
        return 1;  // 发现卡片
    }
    return 0;  // 没有检测到卡
}

// **读取 NFC UID**
hi_u8 rc522_read_card_uid(hi_u8 *uid) {
    hi_u8 status;
    hi_u8 buffer[5];  // UID 长度 = 5 字节
    hi_u8 bufferSize = sizeof(buffer);

    status = rc522_anticoll(buffer);  // 防冲突检测，获取 UID
    if (status == HI_ERR_SUCCESS) {
        memcpy(uid, buffer, 5);  // 复制 UID 数据
        return 1;  // 读取成功
    }
    return 0;  // 读取失败
}

// **请求检测 NFC 卡片**
hi_u8 rc522_request(hi_u8 reqMode, hi_u8 *buffer, hi_u8 *bufferSize) {
    hi_u8 status;
    rc522_write_register(0x0D, 0x07);  // 设置比特帧寄存器
    buffer[0] = reqMode;
    status = rc522_to_card(PCD_TRANSCEIVE, buffer, 1, buffer, bufferSize);
    if ((status != HI_ERR_SUCCESS) || (*bufferSize != 2)) {
        status = HI_ERR_FAILURE;
    }
    return status;
}

// **防冲突检测，获取 UID**
hi_u8 rc522_anticoll(hi_u8 *serial) {
    hi_u8 status;
    hi_u8 check = 0;
    hi_u8 bufferSize = 5;

    rc522_write_register(0x0D, 0x00);  // 关闭比特屏蔽
    serial[0] = PICC_ANTICOLL;
    serial[1] = 0x20;
    
    status = rc522_to_card(PCD_TRANSCEIVE, serial, 2, serial, &bufferSize);
    if (status == HI_ERR_SUCCESS) {
        for (hi_u8 i = 0; i < 4; i++) {
            check ^= serial[i];  // 计算 XOR 校验值
        }
        if (check != serial[4]) {
            status = HI_ERR_FAILURE;
        }
    }
    return status;
}

// **卡片通信**
hi_u8 rc522_to_card(hi_u8 command, hi_u8 *sendData, hi_u8 sendLen, hi_u8 *backData, hi_u8 *backLen) {
    hi_u8 status = HI_ERR_FAILURE;
    hi_u8 irqEn = 0x00;
    hi_u8 waitIRq = 0x00;
    hi_u8 lastBits;
    hi_u8 n;
    hi_u32 i;

    if (command == PCD_AUTHENT) {
        irqEn = 0x12;
        waitIRq = 0x10;
    } else if (command == PCD_TRANSCEIVE) {
        irqEn = 0x77;
        waitIRq = 0x30;
    }

    rc522_write_register(0x02, irqEn | 0x80);
    rc522_write_register(0x04, 0x80);
    rc522_write_register(0x01, PCD_IDLE);

    for (i = 0; i < sendLen; i++) {
        rc522_write_register(0x09, sendData[i]);
    }

    rc522_write_register(0x01, command);
    if (command == PCD_TRANSCEIVE) {
        rc522_write_register(0x0D, 0x80);
    }

    i = 2000;
    do {
        n = rc522_read_register(0x04);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

    if (i != 0) {
        status = HI_ERR_SUCCESS;
    }
    return status;
}
