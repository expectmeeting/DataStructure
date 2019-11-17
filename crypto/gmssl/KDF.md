~~~c

int my_KDF(const char* cdata, int datalen, int keylen, char* retdata)
{
    int nRet = -1;
    unsigned char *pRet;
    unsigned char *pData;
 
    if(cdata==NULL || datalen<=0 || keylen<=0)
    {
        goto err;
    }
 
    if(NULL == (pRet=(unsigned char *)malloc(keylen)))
    {
        goto err;
    }
 
    if(NULL == (pData=(unsigned char *)malloc(datalen+4)))
    {
        goto err;
    }
 
    memset(pRet,  0, keylen);
    memset(pData, 0, datalen+4);
 
    unsigned char cdgst[32]={0}; //摘要
    unsigned char cCnt[4] = {0}; //计数器的内存表示值
    int nCnt  = 1;  //计数器
    int nDgst = 32; //摘要长度
 
    int nTimes = (keylen+31)/32; //需要计算的次数
    int i=0;
    memcpy(pData, cdata, datalen);
    for(i=0; i<nTimes; i++)
    {
        //cCnt
        {
            cCnt[0] =  (nCnt>>24) & 0xFF;
            cCnt[1] =  (nCnt>>16) & 0xFF;
            cCnt[2] =  (nCnt>> 8) & 0xFF;
            cCnt[3] =  (nCnt    ) & 0xFF;
        }
        memcpy(pData+datalen, cCnt, 4);
        sm3(pData, datalen+4, cdgst);
 
        if(i == nTimes-1) //最后一次计算，根据keylen/32是否整除，截取摘要的值
        {
            if(keylen%32 != 0)
            {
                nDgst = keylen%32;
            }
        }
        memcpy(pRet+32*i, cdgst, nDgst);
 
        i    ++;  //
        nCnt ++;  //
    }
 
    if(retdata != NULL)
    {
        memcpy(retdata, pRet, keylen);
    }
 
    nRet = 0;
err:
    if(pRet)
        free(pRet);
    if(pData)
        free(pData);
 
    return nRet;
}
~~~

