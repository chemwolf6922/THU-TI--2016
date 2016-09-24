#include "sci3_dma.h"


//#pragma SET_DATA_SECTION(".sharedRAM")
//uint8_t RX_DATA[size] = {0};
//#pragma SET_DATA_SECTION()
#define SCI3_RX_ADDR ((uint32_t)(&(sciREG3->RD)) + 3)
#define DMA_SCI3_RX  DMA_REQ30
#define SCI_SET_TX_DMA      (1<<16)
#define SCI_SET_RX_DMA      (1<<17)
#define SCI_SET_RX_DMA_ALL  (1<<18)

void sci3_dma_init(u8* RX_DATA){		////////////////////////////////////////////

	uint32  sciRxData;

	g_dmaCTRL  g_dmaCTRLPKT2;

	/*Initialize SCI*/
	sciInit();

	while (((sciREG3->FLR & SCI_TX_INT) == 0U) || ((sciREG3->FLR & 0x4) == 0x4))
    {
	} /* Wait */


	/*Assign DMA request SCI3 receive to Channel 1*/
	dmaReqAssign(DMA_CH1, DMA_SCI3_RX);


	sciRxData = SCI3_RX_ADDR;

	/*Configure control packet for Channel 1*/
	g_dmaCTRLPKT2.SADD      = sciRxData;	        /* source address             */
	g_dmaCTRLPKT2.DADD      = (uint32_t)RX_DATA;	/* destination  addr ss       */
	g_dmaCTRLPKT2.CHCTRL    = 0;                	/* channel control            */
	g_dmaCTRLPKT2.FRCNT	   = size;                  /* frame count                */
	g_dmaCTRLPKT2.ELCNT     = 1;                 	/* element count              */
	g_dmaCTRLPKT2.ELDOFFSET = 0;                	/* element destination offset */
	g_dmaCTRLPKT2.ELSOFFSET = 0;		          	/* element destination offset */
	g_dmaCTRLPKT2.FRDOFFSET = 0;		          	/* frame destination offset   */
	g_dmaCTRLPKT2.FRSOFFSET = 0;                	/* frame destination offset   */
	g_dmaCTRLPKT2.PORTASGN  = PORTB_READ_PORTA_WRITE;
	g_dmaCTRLPKT2.RDSIZE    = ACCESS_8_BIT;		    /* read size                  */
	g_dmaCTRLPKT2.WRSIZE    = ACCESS_8_BIT;	 	    /* write size                 */
	g_dmaCTRLPKT2.TTYPE     = FRAME_TRANSFER;  	    /* transfer type              */
	g_dmaCTRLPKT2.ADDMODERD = ADDR_FIXED;        	/* address mode read          */
	g_dmaCTRLPKT2.ADDMODEWR = ADDR_INC1;      	    /* address mode write         */
	g_dmaCTRLPKT2.AUTOINIT  = AUTOINIT_ON;      	/* autoinit                   */

	/*Set control packet for channel 1*/
	dmaSetCtrlPacket(DMA_CH1, g_dmaCTRLPKT2);

	/*Set dma channel 1 to trigger on hardware request*/
	dmaSetChEnable(DMA_CH1, DMA_HW);

	/*Enable DMA*/
	dmaEnable();

	/*Enable SCI3 Receive DMA Request*/
	sciREG3->SETINT |= SCI_SET_RX_DMA | SCI_SET_RX_DMA_ALL;

}

//u8 sci3_getdata(u8 i){
//	return RX_DATA[i] ;
//}
