#include "bsp_nfc.h"
#include "hi_errno.h"
#include "hi_i2c.h"
#include "NT3H.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief  从Page页中组成NDEF协议的包裹
 * @note
 * @param  *dataBuff: 最终的内容
 * @param  dataBuff_MaxSize: 存储缓冲区的长度
 * @retval
 */
uint32_t get_NDEFDataPackage(uint8_t *dataBuff, const uint16_t dataBuff_MaxSize)
{
    if (dataBuff == NULL || dataBuff_MaxSize <= 0) 
    {
        printf("dataBuff==NULL or dataBuff_MaxSize<=0\r\n");
        return HI_ERR_FAILURE;
    }

    uint8_t userMemoryPageNum = 0; // 用户的数据操作页数

    // 算出要取多少页
    if (dataBuff_MaxSize <= NFC_PAGE_SIZE) 
    {
        userMemoryPageNum = 1; // 1页
    } 
    else 
    {
        // 需要访问多少页
        userMemoryPageNum = (dataBuff_MaxSize / NFC_PAGE_SIZE) + \
                            ((dataBuff_MaxSize % NFC_PAGE_SIZE) >= 0 ? 1 : 0);
    }

    // 内存拷贝
    uint8_t *p_buff = (uint8_t *)malloc(userMemoryPageNum * NFC_PAGE_SIZE);
    if (p_buff == NULL) 
    {
        printf("p_buff == NULL.\r\n");
        return HI_ERR_FAILURE;
    }

    // 读取数据
    for (int i = 0; i < userMemoryPageNum; i++) 
    {
        if (NT3HReadUserData(i) == true) 
        {
            memcpy_s(p_buff + i * NFC_PAGE_SIZE, userMemoryPageNum * NFC_PAGE_SIZE,
                     nfcPageBuffer, NFC_PAGE_SIZE);
        }
    }

    memcpy_s(dataBuff, dataBuff_MaxSize, p_buff, dataBuff_MaxSize);

    free(p_buff);
    p_buff = NULL;

    return HI_ERR_SUCCESS;
}

//NFC初始化
uint32_t nfc_init(void)
{
    uint32_t result;

    // gpio_9 复用为 I2C_SCL
    hi_io_set_pull(HI_IO_NAME_GPIO_9, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_9, HI_IO_FUNC_GPIO_9_I2C0_SCL);
    // gpio_10 复用为 I2C_SDA
    hi_io_set_pull(HI_IO_NAME_GPIO_10, HI_IO_PULL_UP);
    hi_io_set_func(HI_IO_NAME_GPIO_10, HI_IO_FUNC_GPIO_10_I2C0_SDA);

    result = hi_i2c_init(HI_I2C_IDX_0, I2C_RATE_DEFAULT);
    if (result != HI_ERR_SUCCESS) 
    {
        printf("I2C nfc Init status is 0x%x!!!\r\n", result);
        return result;
    }
    printf("I2C nfc Init is succeeded!!!\r\n");

    return true;
}

/**
 * @brief 向NFC卡片写入NDEF数据包
 * @param *dataBuff: 要写入的数据
 * @param dataBuff_Size: 数据长度
 * @retval HI_ERR_SUCCESS: 成功，其他: 失败
 */
uint32_t write_NDEFDataPackage(const uint8_t *dataBuff, uint16_t dataBuff_Size) {
    if (dataBuff == NULL || dataBuff_Size <= 0) {
        printf("dataBuff == NULL 或 dataBuff_Size <= 0\r\n");
        return HI_ERR_FAILURE;
    }

    uint8_t userMemoryPageNum = 0; // 计算需要写入的页数
    if (dataBuff_Size <= NFC_PAGE_SIZE) {
        userMemoryPageNum = 1;
    } else {
        userMemoryPageNum = (dataBuff_Size / NFC_PAGE_SIZE) + ((dataBuff_Size % NFC_PAGE_SIZE) > 0 ? 1 : 0);
    }

    // 写入数据
    for (int i = 0; i < userMemoryPageNum; i++) {
        uint8_t *pageBuffer = (uint8_t *)malloc(NFC_PAGE_SIZE);
        if (pageBuffer == NULL) {
            printf("pageBuffer == NULL.\r\n");
            return HI_ERR_FAILURE;
        }

        // 填充页面数据
        memset(pageBuffer, 0, NFC_PAGE_SIZE);
        memcpy(pageBuffer, dataBuff + i * NFC_PAGE_SIZE, dataBuff_Size > NFC_PAGE_SIZE ? NFC_PAGE_SIZE : dataBuff_Size);

        // 写入页面
        if (!NT3HWriteUserData(i, pageBuffer)) {
            printf("写入页面 %d 失败\r\n", i);
            free(pageBuffer);
            return HI_ERR_FAILURE;
        }

        free(pageBuffer);
    }

    return HI_ERR_SUCCESS;
}