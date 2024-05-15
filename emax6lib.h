/* EMAX6 library                        */
/*         Copyright (C) 2013- by NAIST */
/*          Primary writer: Y.Nakashima */
/*                 nakashim@is.naist.jp */

/*******************************************************************************/
/******************************** Defs *****************************************/
/*******************************************************************************/
#pragma once
#define EMAX6LIB
#include "emax6.h"
#include <sys/types.h>
#include <stdio.h>
#include <stddef.h>

enum { NANOS_ARM, NANOS_DRAIN, NANOS_CONF, NANOS_REGV, NANOS_RANGE, NANOS_LOAD, NANOS_EXEC, NANOS_TOTAL, NANOS_CLASS };

typedef struct {
  Uint  f : 23;
  Uint  e :  8;
  Uint  s :  1;
} f32bit;

typedef struct {
  Uint  e :  6;
  Uint  b :  1;
  Uint  s :  1; /* lower 8bit */
  Uint dm : 24; /* dummy for >gcc9 */
} wu7bit;

typedef struct {
  Uint  e :  7;
  Uint  s :  1; /* lower 8bit */
  Uint dm : 24; /* dummy for >gcc9 */
} wu8bit;

typedef struct {
  Uchar u[8];
} u64bit;

#define abs(a)    ((a)>  0 ? (a) :-(a)    )
#define ad(a,b)   ((a)<(b)?(b)-(a):(a)-(b))
#define ss(a,b)   ((a)<(b)?   0   :(a)-(b))

/* dma_ctrl_space */
/* regmap of zynq ultrascale+ */
/* https://japan.xilinx.com/html_docs/registers/ug1087/ug1087-zynq-ultrascale-registers.html#_module_summary.html */
struct dma_ctrl {
  /*   Register Name		   Address	Width	Type	Reset Value	Description */
  Uint ZDMA_ERR_CTRL;    	/* 0x00000000	32	mixed	0x00000001	Enable/Disable a error response */
  Uint dmy0[63];
  Uint ZDMA_CH_ISR;      	/* 0x00000100	32	mixed	0x00000000	Interrupt Status Register for intrN. This is a sticky register that holds the value of the interrupt until cleared by a value of 1. */
  Uint ZDMA_CH_IMR;      	/* 0x00000104	32	mixed	0x00000FFF	Interrupt Mask Register for intrN. This is a read-only location and can be atomically altered by either the IDR or the IER. */
  Uint ZDMA_CH_IEN;      	/* 0x00000108	32	mixed	0x00000000	Interrupt Enable Register. A write of to this location will unmask the interrupt. (IMR: 0) */
  Uint ZDMA_CH_IDS;      	/* 0x0000010C	32	mixed	0x00000000	Interrupt Disable Register. A write of one to this location will mask the interrupt. (IMR: 1) */
  Uint ZDMA_CH_CTRL0;    	/* 0x00000110��	32	mixed	0x00000080	Channel Control Register 0 */
	/*   Field Name  Bits  Type Reset Value  Description            */
	/*   Reserved	 31:8  raz  0x0	         Reseved for future use */
	/*   OVR_FETCH	    7  rw   0x1	         0: DMA channel is not allowed to over-fetch on SRC */
	/*                                       1: DMA channel is allowed to over-fetch */
	/*                                       This field must remain stable while DMA Channel is enabled */
	/*   POINT_TYPE	    6  rw   0x0	       ��0: Simple mode DMA. Descriptor (DMA command) from APB register space. */
	/*                                       1: Scatter-gather mode DMA. Descriptor are stored in Memory. */
	/*                                       This field must remain stable while DMA Channel is enabled */
	/*   MODE	  5:4  rw   0x0	         00: Normal read & write DMA (default) */
	/*                                       01: Write only */
	/*                                       (uses data from WR_DMA_DATA*) */
	/*                                       10: Read only */
	/*                                       11: Reseved for future use */
	/*                                       This field must remain stable while DMA Channel is enabled */
	/*   RATE_CTRL	    3  rw   0x0	         Enable/Disable rate control */
	/*                                       0: rate control is disabled */
	/*                                       1: rate control is enabled, use rate control count to schedule AXI transaction (Read) */
	/*                                       This field must remain stable while DMA Channel is enabled */
	/*   CONT_ADDR	    2  rw   0x0	         Coming out of pause */
	/*                                       0: use continuous address(calculated from previous DSCR Addr) to fetch next descriptor */
	/*                                       1: use address specified in Start Address Register to fetch next descriptor on both SRC & DST side */
	/*   CONT	    1  wo   0x0	         Setting to 1 unpause (restarts from current position) the pauseed DMA */
	/*                                       SW sets this 1 to trigger. When DMA is unpaused , then hardware clears this bit. SW clear has no effect on it */
	/*   Reserved	    0  raz  0x0	         Reseved for future use */

  Uint ZDMA_CH_CTRL1;    	/* 0x00000114	32	mixed	0x000003FF	Channel Flow Control Register */
  Uint ZDMA_CH_FCI;      	/* 0x00000118	32	mixed 	0x00000000	Channel Control Register 1 */
  Uint ZDMA_CH_STATUS;   	/* 0x0000011C��	32	mixed	0x00000000	Channel Status Register */
	/*   Field Name  Bits  Type Reset Value  Description            */
	/*   Reserved    31:2  raz  0x0          Reseved for future use */
	/*   STATE        1:0  ro   0x0        ��00: Done with no error (as a result Enable bit is cleared by HW) */
	/*                                       01: paused with no error (as a result Enable bit remains set). */
	/*                                       10: DMA is busy transferring */
	/*                                     ��11: DMA done with error (error condition captured in otherregisters) */
	/*                                       This bit is cleared by HW when Enable or Unpause is set to 1. */

  Uint ZDMA_CH_DATA_ATTR;	/* 0x00000120	32	mixed	0x0483D20F	Channel DATA AXI parameter Register */
  Uint ZDMA_CH_DSCR_ATTR;	/* 0x00000124	32	mixed	0x00000000	Channel DSCR AXI parameter Register */
  Uint ZDMA_CH_SRC_DSCR_WORD0;	/* 0x00000128��	32	rw	0x00000000	SRC DSCR Word 0 */
  Uint ZDMA_CH_SRC_DSCR_WORD1;  /* 0x0000012C��	32	mixed	0x00000000	SRC DSCR Word 1 */
  Uint ZDMA_CH_SRC_DSCR_WORD2;  /* 0x00000130��	32	mixed	0x00000000	SRC DSCR Word 2 */
	/*   Field Name  Bits  Type Reset Value  Description            */
	/*   Reserved	 31:30 raz  0x0	         Reserved for furure use */
	/*   SIZE	 29:0  rw   0x0	         Buffer size in bytes (1G=2^30) */

  Uint ZDMA_CH_SRC_DSCR_WORD3;  /* 0x00000134	32	mixed	0x00000000	SRC DSCR Word 3 */
  Uint ZDMA_CH_DST_DSCR_WORD0;  /* 0x00000138��	32	rw	0x00000000	DST DSCR Word 0 */
  Uint ZDMA_CH_DST_DSCR_WORD1;  /* 0x0000013C��	32	mixed	0x00000000	DST DSCR Word 1 */
  Uint ZDMA_CH_DST_DSCR_WORD2;  /* 0x00000140��	32	mixed	0x00000000	DST DSCR Word 2 */
	/*   Field Name  Bits  Type Reset Value  Description            */
	/*   Reserved	 31:30 raz  0x0	         Reserved for furure use */
	/*   SIZE	 29:0  rw   0x0	         Buffer size in bytes (1G=2^30) */

  Uint ZDMA_CH_DST_DSCR_WORD3;  /* 0x00000144	32	mixed	0x00000000	DST DSCR Word 3 */
  Uint ZDMA_CH_WR_ONLY_WORD0;   /* 0x00000148	32	rw	0x00000000	Write Only Data Word 0 */
  Uint ZDMA_CH_WR_ONLY_WORD1;   /* 0x0000014C	32	rw	0x00000000	Write Only Data Word 1 */
  Uint ZDMA_CH_WR_ONLY_WORD2;   /* 0x00000150	32	rw	0x00000000	Write Only Data Word 2 */
  Uint ZDMA_CH_WR_ONLY_WORD3;   /* 0x00000154	32	rw	0x00000000	Write Only Data Word 3 */
  Uint ZDMA_CH_SRC_START_LSB;   /* 0x00000158	32	rw	0x00000000	SRC DSCR Start Address LSB Regiser */
  Uint ZDMA_CH_SRC_START_MSB;   /* 0x0000015C	32	mixed	0x00000000	SRC DSCR Start Address MSB Regiser */
  Uint ZDMA_CH_DST_START_LSB;   /* 0x00000160	32	rw	0x00000000	DST DSCR Start Address LSB Regiser */
  Uint ZDMA_CH_DST_START_MSB;   /* 0x00000164	32	mixed	0x00000000	DST DSCR Start Address MSB Regiser */
  Uint dmy1[9];
  Uint ZDMA_CH_RATE_CTRL;       /* 0x0000018C	32	mixed	0x00000000	Rate Control Count Register */
  Uint ZDMA_CH_IRQ_SRC_ACCT;    /* 0x00000190	32	mixed	0x00000000	SRC Interrupt Account Count Register */
  Uint ZDMA_CH_IRQ_DST_ACCT;    /* 0x00000194	32	mixed	0x00000000	DST Interrupt Account Count Register */
  Uint dmy2[26];
  Uint ZDMA_CH_CTRL2;  		/* 0x00000200��	32	mixed	0x00000000	zDMA Control Register 2 */
	/*   Field Name  Bits  Type Reset Value  Description            */
	/*   Reserved	 31:1  raz  0x0	         Reseved for future use */
	/*   EN	            0  rw   0x0	         Channel is enabled */
	/*                                       SW sets this 1 to trigger. If DMA channel is in pause and this bit is cleared by software */
	/*                                       then DMA channel goes to disable state when SW set CONT bit. */
	/*                                       HW clears this flag after finishing DMA opertion. */

  /* Simple Mode */
  /* 1. ZDMA_CH_STATUS �쥸�������ɤ߽Ф���STATE��00�ޤ���11(DMA��IDLE���ơ���) �ˤʤ�ޤ��Ԥ�. */
  /*    DMA��PAUSE���ơ��Ȥξ�硢�֥���ͥ�����ߡפ�����������˽��ä� PAUSE ���ơ��Ȥ�λ. */
  /* 2. ZDMA_CH_CTRL0�쥸������ POINT_TYPE (�ӥå� 6) �� 0 ������ */
  /*    �ǡ����������Хåե��Υ��ɥ쥹�� LSB �� ZDMA_CH_SRC_DSCR_WORD0�쥸�����˽񤭹��� */
  /*    �ǡ����������Хåե��Υ��ɥ쥹�� MSB �� ZDMA_CH_SRC_DSCR_WORD1�쥸�����˽񤭹��� */
  /* 3. �ǡ����ǥ��ƥ��͡������Хåե��Υ��ɥ쥹�� LSB �� ZDMA_CH_DST_DSCR_WORD0�쥸�����˽񤭹��� */
  /*    �ǡ����ǥ��ƥ��͡������Хåե��Υ��ɥ쥹�� MSB �� ZDMA_CH_DST_DSCR_WORD1�쥸�����˽񤭹��� */
  /* 4. Simple Mode�Ǥϡ�SRC�ȥ�󥶥�����󥵥�����DST�ȥ�󥶥�����󥵥�����ξ�������ꤹ��ɬ�פ����� */
  /*    DMA �� SRC�ȥ�󥶥�����󥵥�������Ѥ��ޤ�����ξ���Υ쥸���������ꤷ�Ƥ���ɬ�פ����� */
  /*    ZDMA_CH_SRC_DSCR_WORD2 �쥸�����˥������ǡ�����������񤭹��� */
  /*    ZDMA_CH_DST_DSCR_WORD2 �쥸�����˥ǥ��ƥ��͡������ȥ�󥶥�����󥵥�����񤭹��� */
  /*    SRC�ȥ�󥶥�����󥵥�����DST�ȥ�󥶥�����󥵥�����ɬ��Ʊ�� */
  /* 5. ɬ�פ˱���,ZDMA_CH_DST_DSCR_WORD3�����ZDMA_CH_SRC_DSCR_WORD3�쥸������INTR��1�˥��åȤ������ߤ�ͭ���ˤ��� */
  /* 6. ����������ӥǥ��ƥ��͡������Хåե�������å��女�ҡ����ȤȤ��Ƴ�����Ƥ��Ƥ��뤫�ե�å��夵��Ƥ�����, */
  /*    COHRNT�򥻥åȤ���ɬ�פϤʤ�. ����ʳ��ξ��,����������ӥǥ��ƥ��͡������Хåե�������å��女�ҡ����ȤȤ��� */
  /*    ������Ƥ��Ƥ��ʤ����ޤ��ϥե�å��夵��Ƥ��ʤ���硢ZDMA_CH_SRC_DSCR_WORD3�����ZDMA_CH_DST_DSCR_WORD3�쥸������ */
  /*    ���줾�� COHRNT�򥻥åȤ���. COHRNT�ӥåȤ� LPD-DMA �ξ��Τ�ͭ��. FPD-DMA�ϥ��ҡ���󥷤򥵥ݡ��Ȥ��ʤ�. */
  /* 7. ZDMA_CH_CTRL2�쥸������EN�ӥåȤ򥻥åȤ���,DMAž���˻��Ѥ���DMA����ͥ��ͭ���ˤ���. */
  /*    DMA��ͭ���ˤ����塢�֥��顼���פ˼������顼��������å�����. */
};

/* reg_ctrl */
enum { EXRING_IDLE, EXRING_BUSY};
enum { LMRING_IDLE, LMRING_BUSY};
enum { CMD_NOP, CMD_RESET, CMD_SCON, CMD_EXEC};
struct reg_ctrl {
  struct i0 {
    Ull  stat; /* +0000 bit15-12:LMM_SIZE, bit11-8:EMAX_DEPTH, bit7-4:LMRING, bit3-0:EXRING */
    Uint mcid; /* +0008 maximum chip-ID of IMAX (<EMAX_NCHIP) to be chained (activated) */
    Uint dmy0;
    Uint cmd;  /* +0010 host writes Ull cmd then chip# is propagated to succesors */
  /*Uint cid;*//* +0012 chip# ( set by write to cmd ) */
    Uint dmy1;
    Ull  dmy2;
    Ull  adtr; /* +0020 */
    Ull  dmy3;
    Ull  csel; /* +0030 */
    Ull  dmrp; /* +0038 DMAREAD-PREF */
    Ull  dmy4[1016];
    struct conf                    conf[AMAP_DEPTH][EMAX_WIDTH];  /* +2000-3fff */
    struct {Ull  br[UNIT_WIDTH];}  breg[AMAP_DEPTH][EMAX_WIDTH];  /* +4000-5fff *//* unit[cid][EMAX_DEPTH].breg[x][EMAX_WIDTH].br[UNIT_WIDTH] is used */
    struct {Uint ea0b ; /* ea0 base   (for avoiding ld-mask-st, should be extended to 32bits (lower 18bit is available)) */
      /*Ull  dmy0 :14;*/
        Uint ea0o ; /* ea0 offset (for avoiding ld-mask-st, should be extended to 32bits (lower 18bit is available)) */
      /*Ull  dmy1 :14;*/
        Uint ea1b ; /* ea1 base   (for avoiding ld-mask-st, should be extended to 32bits (lower 18bit is available)) */
      /*Ull  dmy2 :14;*/
        Uint ea1o ; /* ea1 offset (for avoiding ld-mask-st, should be extended to 32bits (lower 18bit is available)) */
      /*Ull  dmy3 :14;*/
        Uint top  ; /* LMM-top virtual-address */
      /*Ull  dmy4 : 1;*/
        Uint bot  ; /* LMM-bot virtual-address */
      /*Ull  dmy5 : 1;*/
        Ull  dmy6 ;}           addr[AMAP_DEPTH][EMAX_WIDTH];  /* +6000-7fff */
    struct {Ull  reg[UNIT_WIDTH];} lddmrw[AMAP_DEPTH][EMAX_WIDTH];/* +8000-9fff *//* lddmq/trans-r,lddmq-w */
    Ull dmy5[3072]; /* +a000-ffff */
  } i[EMAX_NCHIP]; /* 0000-ffff */
};

/* emax6 host control */
enum { STATUS_IDLE, STATUS_CONF, STATUS_SCON, STATUS_REGV, STATUS_RANGE, STATUS_DRAIN, STATUS_LOAD, STATUS_START, STATUS_EXEC, STATUS_TERM };

struct emax6 { /* host status of EMAX6 */
  volatile Ull   dma_ctrl;  /* struct dma_ctrl *dma_ctrl  DMA control */
  volatile Ull   reg_ctrl;  /* struct reg_ctrl *reg_ctrl  REG control */

  Ull   status            : 4;
  Ull   csel_save         : 2;
  Ull   last_conf            ; /* for insn_reuse */
  Ull   lmmic             : 1; /* 0:lmm[0] is curent, 1:lmm[1] is current */
  Ull   lmmio             : 1; /* 0:lmm[0] is prev,   1:lmm[1] is prev    */
  Ull   mapdist           : 6; /* specified mapdist */
  Ull   lastdist          : 6; /* lastdist for DYNAMIC_SCON */
  struct lmmi lmmi[EMAX_NCHIP][AMAP_DEPTH][EMAX_WIDTH][2]; /* lmmi for host (len/ofs/top are resolved) */
  Ull   lmmi_bitmap[EMAX_WIDTH];      /* based on lmmi[*][EMAX_WIDTH][2].v */
  Uchar lmmd[AMAP_DEPTH][EMAX_WIDTH]; /* chip#7,6,..,0:clean, 1:dirty, exec��store�ս��1, drainľ��0 */

#ifndef IGNORE_LMMI_BLKGATHER
  Ull   plist                ; /* pointer-list */
  Ull   blkcount          : 7; /* active block number */
  Ull   blksize           : 9; /* 1:64 2:128 3:256 dwords */
  Ull   lmmblktop            ; /* LMM-addr     for LDRQ(blk>0) */
  Ull   lmmblklen            ; /* total dwords for LDRQ(blk>0) */
#endif

  Ull   rw                    ; /* 0:load(mem->lmm), 1:store(lmm->mem)      */
  Ull   ddraddr               ; /* ddr-address                              */
  Ull   lmmaddr               ; /* lmm-address                              */
  Ull   dmalen                ; /* dma-length                               */

#ifndef IGNORE_LDDMQ_HANDSHAKE
  Ull   fsm_busy          : 1; /* for LDDMQ and TR */
  Ull   lmwd_valid        : 1; /* for LDDMQ */
  Ull   tcureg_valid      : 1; /* fsm->ARM   0 -> 1 -> 1 -> 0 -> 0 -> 0                              */
  Ull   tcureg_ready      : 1; /* fsm<-ARM   0 -> 0 -> 1 -> 0 -> 0 -> 0                              */
  Ull   tcureg_last       : 1; /* fsm->ARM   0 -> 0 -> 0 -> 1 -> 1 -> 0                              */
  Ull   tcureg_term       : 1; /* fsm<-ARM   0 -> 0 -> 0 -> 0 -> 1 -> 0                              */
  Ull   tcureg[UNIT_WIDTH]   ; /* tcu-data        of tcu                       v                     */
                               /* from ARM:  svc 0x1010 ... tcureg_valid->x0                         */
                               /* from ARM:  svc 0x1011 ... 1->tcureg_ready                          */
                               /* from ARM:  svc 0x1012 ... tcureg_last->x0                          */
                               /* from ARM:  svc 0x1013 ... 1->tcureg_term                           */
                               /* from ARM:  svc 0x1014 ... tcureg[3:0]->x3,2,1,0                    */
#endif
} emax6;

volatile struct emax_info {
  Ull  dma_phys;     // kern-phys
  Ull  dma_vadr;     // not used
  Ull  dma_mmap;     // user-virt Contiguous 64K register space
  Ull  reg_phys;     // kern-phys
  Ull  reg_vadr;     // not used
  Ull  reg_mmap;     // user-virt Contiguous 4GB space including LMM space
  Ull  lmm_phys;     // kern-phys
  Ull  lmm_vadr;     // not used
  Ull  lmm_mmap;     // user-virt Contiguous 2GB space for LMM space
  Ull  ddr_phys;     // kern-phys
  Ull  ddr_vadr;     // not used
  Ull  ddr_mmap;     // user-virt Contiguous 2GB space in DDR-high-2GB space
  int  driver_use_1;
  int  driver_use_2;
} emax_info;

/*  ... for ARMSIML only */
#define DMA_BASE2_PHYS	 0x50000000
#define REG_BASE2_PHYS	 0x50100000
#define REG_CONF_OFFS    0x00002000
#define REG_BREG_OFFS    0x00004000
#define REG_ADDR_OFFS    0x00006000
#define REG_LDDM_OFFS    0x00008000
#define REG_AREA_MASK    0x0000ffff
#define LMM_BASE2_PHYS 	 0x60000000
#define MEM_VALID_ADDR	 0xafffffff

#ifndef NO_EMAX6LIB_BODY

#ifdef ARMZYNQ
/*******************************************************************************/
/******************************** ZYNQ-COMMON **********************************/
/*******************************************************************************/

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <linux/ioctl.h>

#define DMA_BASE_PHYS	 0x00000000fd500000LL
/*  ... fixed */
#define DMA_MMAP_SIZE	 0x0000000000010000LL
/*  ... 64KB  */
#define REG_BASE_PHYS	 0x0000000400000000LL
/*  ... fixed */
#define REG_MMAP_SIZE	 0x0000000100000000LL
/*  ... 4GB(including REGS) */
#define LMM_BASE_PHYS	 0x0000000480000000LL
/*  ... fixed */
#define DDR_BASE_PHYS	 0x0000000800000000LL
/*  ... fixed */
#define DDR_MMAP_SIZE	 0x0000000080000000LL
/*  ... 2GB   */

#define EMAX_IOC_MAGIC  60
/* Please use a different 8-bit number in your code */
#define EMAX_IORESET			_IO( EMAX_IOC_MAGIC, 0)
#define EMAX_GET_ACPMEM			_IOWR(EMAX_IOC_MAGIC,  1, unsigned long)
#define EMAX_IOC_MAXNR 2

static int filter(struct dirent *dir)
{
  return dir->d_name[0] == '.' ? 0 : 1;
}

static void trim(char *d_name)
{
  char *p = strchr(d_name, '\n');
  if (p != NULL) *p = '\0';
}

static int is_target_dev(char *d_name, char *target)
{
  char path[32];
  char name[32];
  FILE *fp;
  sprintf(path, "/sys/class/uio/%s/name", d_name);
  if ((fp = fopen(path, "r")) == NULL) return 0;
  if (fgets(name, sizeof(name), fp) == NULL) {
    fclose(fp);
    return 0;
  }
  fclose(fp);
  if (strcmp(name, target) != 0) return 0;
  return 1;
}

static int get_reg_size(char *d_name)
{
  char path[32];
  char size[32];
  FILE *fp;
  sprintf(path, "/sys/class/uio/%s/maps/map0/size", d_name);
  if ((fp = fopen(path, "r")) == NULL) return 0;
  if (fgets(size, sizeof(size), fp) == NULL) {
    fclose(fp);
    return 0;
  }
  fclose(fp);
  return strtoull(size, NULL, 16);
}

emax6_open();
#endif

/*******************************************************************************/
/******************************** Timer ****************************************/
/*******************************************************************************/

Ull nanosec_sav;
Ull nanosec[NANOS_CLASS];

reset_nanosec();
get_nanosec(int class);
show_nanosec();

/*******************************************************************************/
/******************************** DMA-START ************************************/
/*******************************************************************************/

#if !defined(EMAXNC)
emax6_check_lmmi_and_dma(int mode, int phase, int lastdist, int c, int i, int j);
emax6_kick_dma(int j);

/*******************************************************************************/
/******************************** EMAX6-START **********************************/
/*******************************************************************************/

/* lmmwb=0: if lmm never be written back to DDR */
emax6_pre_with_keep_cache();
/* lmmwb=1: if lmm may   be written back to DDR */
emax6_pre_with_drain_cache();

#endif

/*******************************************************************************/
/******************************** EMAX6 NCLIB (no conv-c2c)*********************/
/*******************************************************************************/

void cex(Uint op_cx, Ull *ex, Ull c3, Ull c2, Ull c1, Ull c0, Ushort pattern);
void ex4(Uint op_ex1, Ull *d, Ull *r1, Uint exp1, Ull *r2, Uint exp2, Ull *r3, Uint exp3, Uint op_ex2, Ull *r4, Uint op_ex3, Ull *r5);
int convf32tou7(Uchar *out, float in);
int convf32tou8(Uchar *out, float in);
int convu8tof32(float *out, Uchar in);

Ull urand(int no);
Ull shfl(Ull in, Ull r);

int enable_x11;    /* 0 or set in extern */

void x11_softu64_dist(float, float);
int softu64(int stage, Ull *o1, Ull *o2, Ull *o3, Ull r1, Ull r2, Ull r3, Ull r4);
inline Ull __attribute__((always_inline)) exm(Ull s, Uchar exp)
{
  switch (exp) {
  case EXP_H3210: return ( s );
  case EXP_H1010: return ((s<<32&0xffffffff00000000LL) | (s    &0x00000000ffffffffLL));
  case EXP_H3232: return ((s    &0xffffffff00000000LL) | (s>>32&0x00000000ffffffffLL));
  case EXP_B7632: return ((s>> 8&0x00ff000000ff0000LL) | (s>>16&0x000000ff000000ffLL));
  case EXP_B5410: return ((s<< 8&0x00ff000000ff0000LL) | (s    &0x000000ff000000ffLL));
  default:        return ( s );
  }
}

int exe(Uint op_ex1, Ull *d, Ull s1, Uint exp1, Ull s2, Uint exp2, Ull s3, Uint exp3, Uint op_ex2, Ull r4, Uint op_ex3, Ull r5);

void mex(Uint op_mex2, Uchar **d2, Uchar *base2, Ull ofs2, Uint op_mex1, Uchar **d1, Uchar *base1, Ull ofs1, Ull limit, Ull s2, Ull s1);
inline Ull __attribute__((always_inline)) eam(Ull ofs, Uchar msk)
{
  switch (msk) {
  case  MSK_D0: return (ofs);
  case  MSK_W1: return (ofs>>32);
  case  MSK_W0: return (ofs&0x00000000ffffffffLL);
  case  MSK_H3: return (ofs>>48&0x000000000000ffffLL);
  case  MSK_H2: return (ofs>>32&0x000000000000ffffLL);
  case  MSK_H1: return (ofs>>16&0x000000000000ffffLL);
  case  MSK_H0: return (ofs&0x000000000000ffffLL);
  case  MSK_B7: return (ofs>>56&0x00000000000000ffLL);
  case  MSK_B6: return (ofs>>48&0x00000000000000ffLL);
  case  MSK_B5: return (ofs>>40&0x00000000000000ffLL);
  case  MSK_B4: return (ofs>>32&0x00000000000000ffLL);
  case  MSK_B3: return (ofs>>24&0x00000000000000ffLL);
  case  MSK_B2: return (ofs>>16&0x00000000000000ffLL);
  case  MSK_B1: return (ofs>>8&0x00000000000000ffLL);
  case  MSK_B0: return (ofs&0x00000000000000ffLL);
  default: printf("emax6lib: eag: undefined msk=%d\n", msk); return (0LL);;
  }
}

void eag(Ull *adr, Ull base, Ull ofs);

void mop(Uint op_mm, Ull ex, Ull *d, Ull base, Ull offset, Uchar msk, Ull top, Uint len, Uint blk, Uchar force, Ull ptop, Uint plen);
void mo4(Uint op_mm, Ull ex, Ull *d, Ull base, Ull offset, Uchar msk, Ull top, Uint len, Uint blk, Uchar force, Ull ptop, Uint plen);

int emax6_unaligned_load_valid; /* mop(BR[][][1]adr+8); mop(BR[][][0]adr);Ϣ³��������ꤷ,1�ξ��high��ͭ��,0�ξ��̵��  */
Ull emax6_unaligned_load_high;  /* mop(BR[][][1]adr+8); mop(BR[][][0]adr);Ϣ³��������ꤷ,high¦��켡��¸��low¦��merge */

void mmp(Uint op_mm, Ull ex, Ull *d, Ull adr, Ull top, Uint len, Uint blk);
#endif
