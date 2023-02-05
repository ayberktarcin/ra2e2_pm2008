/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (6)
#endif
/* ISR prototypes */
void r_icu_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void gpt_counter_overflow_isr(void);
void sci_uart_eri_isr(void);
void sci_uart_tei_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_ICU_IRQ1 ((IRQn_Type) 1) /* ICU IRQ1 (External pin interrupt 1) */
#define ICU_IRQ1_IRQn          ((IRQn_Type) 1) /* ICU IRQ1 (External pin interrupt 1) */
#define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 4) /* SCI9 RXI (Received data full) */
#define SCI9_RXI_IRQn          ((IRQn_Type) 4) /* SCI9 RXI (Received data full) */
#define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 5) /* SCI9 TXI (Transmit data empty) */
#define SCI9_TXI_IRQn          ((IRQn_Type) 5) /* SCI9 TXI (Transmit data empty) */
#define VECTOR_NUMBER_GPT6_COUNTER_OVERFLOW ((IRQn_Type) 6) /* GPT6 COUNTER OVERFLOW (Overflow) */
#define GPT6_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 6) /* GPT6 COUNTER OVERFLOW (Overflow) */
#define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 7) /* SCI9 ERI (Receive error) */
#define SCI9_ERI_IRQn          ((IRQn_Type) 7) /* SCI9 ERI (Receive error) */
#define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 14) /* SCI9 TEI (Transmit end) */
#define SCI9_TEI_IRQn          ((IRQn_Type) 14) /* SCI9 TEI (Transmit end) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
