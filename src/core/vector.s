.section .vector
.globl vector_start
vector_start:
.int 0		/*V0	reset sp*/
.int _start /*V1	reset pc*/
.int bus_err/*V2	bus error*/
.int 0		/*V3	addr error*/
.int 0		/*V4	illegal instruction*/
.int 0		/*V5	divide by zero*/
.int 0		/*V6	chk, chk2*/
.int 0		/*V7	trap*/
.int 0		/*V8	privilege violation*/
.int 0		/*V9	trace*/
.int 0		/*V10	line 1010*/
.int 0		/*V11	line 1110*/
.int 0		/*V12	unassigned*/
.int 0		/*V13	coprocessor violation*/
.int 0		/*V14	format error*/
.int 0		/*V15	uninitialized interrupt*/
.int 0		/*V16	unassigned*/
.int 0		/*V17	unassigned*/
.int 0		/*V18	unassigned*/
.int 0		/*V19	unassigned*/
.int 0		/*V20	unassigned*/
.int 0		/*V21	unassigned*/
.int 0		/*V22	unassigned*/
.int 0		/*V23	unassigned*/
.int 0		/*V24	spurious*/
.int 0		/*V25	lvl 1 autovector*/
.int 0		/*V26	lvl 2 autovector*/
.int serial_irq	/*V27	lvl 3 autovector*/
.int 0		/*V28	lvl 4 autovector*/
.int 0		/*V29	lvl 5 autovector*/
.int 0		/*V30	lvl 6 autovector*/
.int 0		/*V31	lvl 7 autovector*/
.int syscall_hdl/*V32	trap 0*/
.int 0		/*V33	trap 1*/
.int 0		/*V34	trap 2*/
.int 0		/*V35	trap 3*/
.int 0		/*V36	trap 4*/
.int 0		/*V37	trap 5*/
.int 0		/*V38	trap 6*/
.int 0		/*V39	trap 7*/
.int 0		/*V40	trap 8*/
.int 0		/*V41	trap 9*/
.int 0		/*V42	trap 10*/
.int 0		/*V43	trap 11*/
.int 0		/*V44	trap 12*/
.int 0		/*V45	trap 13*/
.int 0		/*V46	trap 14*/
.int 0		/*V47	trap 15*/
.int 0		/*V48	fpcp branch*/
.int 0		/*V49	fpcp inexact*/
.int 0		/*V50	fpcp divide by zero*/
.int 0		/*V51	fpcp underflow*/
.int 0		/*V52	fpcp operand error*/
.int 0		/*V53	fpcp overflow*/
.int 0		/*V54	fpcp nan*/
.int 0		/*V55	unassigned*/
.int 0		/*V56	reset pc*/
.int 0		/*V57	reset pc*/
.int 0		/*V58	reset pc*/
.int 0		/*V59	reset pc*/
.int 0		/*V60	reset pc*/
.int 0		/*V61	reset pc*/
.int 0		/*V62	reset pc*/
.int 0		/*V63	reset pc*/
.int 0		/*V64	reset pc*/
.int 0		/*V65	reset pc*/
.int 0		/*V66	reset pc*/
.int 0		/*V67	reset pc*/
.int 0		/*V68	reset pc*/
.int 0		/*V69	reset pc*/
.int 0		/*V70	reset pc*/
.int 0		/*V71	reset pc*/
.int 0		/*V72	reset pc*/
.int 0		/*V73	reset pc*/
.int 0		/*V74	reset pc*/
.int 0		/*V75	reset pc*/
.int 0		/*V76	reset pc*/
.int 0		/*V77	reset pc*/
.int 0		/*V78	reset pc*/
.int 0		/*V79	reset pc*/
.int 0		/*V80	reset pc*/
.int 0		/*V81	reset pc*/
.int 0		/*V82	reset pc*/
.int 0		/*V83	reset pc*/
.int 0		/*V84	reset pc*/
.int 0		/*V85	reset pc*/
.int 0		/*V86	reset pc*/
.int 0		/*V87	reset pc*/
.int 0		/*V88	reset pc*/
.int 0		/*V89	reset pc*/
.int 0		/*V90	reset pc*/
.int 0		/*V91	reset pc*/
.int 0		/*V92	reset pc*/
.int 0		/*V93	reset pc*/
.int 0		/*V94	reset pc*/
.int 0		/*V95	reset pc*/
.int 0		/*V96	reset pc*/
.int 0		/*V97	reset pc*/
.int 0		/*V98	reset pc*/
.int 0		/*V99	reset pc*/
.int 0		/*V100	reset pc*/
.int 0		/*V101	reset pc*/
.int 0		/*V102	reset pc*/
.int 0		/*V103	reset pc*/
.int 0		/*V104	reset pc*/
.int 0		/*V105	reset pc*/
.int 0		/*V106	reset pc*/
.int 0		/*V107	reset pc*/
.int 0		/*V108	reset pc*/
.int 0		/*V109	reset pc*/
.int 0		/*V110	reset pc*/
.int 0		/*V111	reset pc*/
.int 0		/*V112	reset pc*/
.int 0		/*V113	reset pc*/
.int 0		/*V114	reset pc*/
.int 0		/*V115	reset pc*/
.int 0		/*V116	reset pc*/
.int 0		/*V117	reset pc*/
.int 0		/*V118	reset pc*/
.int 0		/*V119	reset pc*/
.int 0		/*V120	reset pc*/
.int 0		/*V121	reset pc*/
.int 0		/*V122	reset pc*/
.int 0		/*V123	reset pc*/
.int 0		/*V124	reset pc*/
.int 0		/*V125	reset pc*/
.int 0		/*V126	reset pc*/
.int 0		/*V127	reset pc*/
.int 0		/*V128	reset pc*/
.int 0		/*V129	reset pc*/
.int 0		/*V130	reset pc*/
.int 0		/*V131	reset pc*/
.int 0		/*V132	reset pc*/
.int 0		/*V133	reset pc*/
.int 0		/*V134	reset pc*/
.int 0		/*V135	reset pc*/
.int 0		/*V136	reset pc*/
.int 0		/*V137	reset pc*/
.int 0		/*V138	reset pc*/
.int 0		/*V139	reset pc*/
.int 0		/*V140	reset pc*/
.int 0		/*V141	reset pc*/
.int 0		/*V142	reset pc*/
.int 0		/*V143	reset pc*/
.int 0		/*V144	reset pc*/
.int 0		/*V145	reset pc*/
.int 0		/*V146	reset pc*/
.int 0		/*V147	reset pc*/
.int 0		/*V148	reset pc*/
.int 0		/*V149	reset pc*/
.int 0		/*V150	reset pc*/
.int 0		/*V151	reset pc*/
.int 0		/*V152	reset pc*/
.int 0		/*V153	reset pc*/
.int 0		/*V154	reset pc*/
.int 0		/*V155	reset pc*/
.int 0		/*V156	reset pc*/
.int 0		/*V157	reset pc*/
.int 0		/*V158	reset pc*/
.int 0		/*V159	reset pc*/
.int 0		/*V160	reset pc*/
.int 0		/*V161	reset pc*/
.int 0		/*V162	reset pc*/
.int 0		/*V163	reset pc*/
.int 0		/*V164	reset pc*/
.int 0		/*V165	reset pc*/
.int 0		/*V166	reset pc*/
.int 0		/*V167	reset pc*/
.int 0		/*V168	reset pc*/
.int 0		/*V169	reset pc*/
.int 0		/*V170	reset pc*/
.int 0		/*V171	reset pc*/
.int 0		/*V172	reset pc*/
.int 0		/*V173	reset pc*/
.int 0		/*V174	reset pc*/
.int 0		/*V175	reset pc*/
.int 0		/*V176	reset pc*/
.int 0		/*V177	reset pc*/
.int 0		/*V178	reset pc*/
.int 0		/*V179	reset pc*/
.int 0		/*V180	reset pc*/
.int 0		/*V181	reset pc*/
.int 0		/*V182	reset pc*/
.int 0		/*V183	reset pc*/
.int 0		/*V184	reset pc*/
.int 0		/*V185	reset pc*/
.int 0		/*V186	reset pc*/
.int 0		/*V187	reset pc*/
.int 0		/*V188	reset pc*/
.int 0		/*V189	reset pc*/
.int 0		/*V190	reset pc*/
.int 0		/*V191	reset pc*/
.int 0		/*V192	reset pc*/
.int 0		/*V193	reset pc*/
.int 0		/*V194	reset pc*/
.int 0		/*V195	reset pc*/
.int 0		/*V196	reset pc*/
.int 0		/*V197	reset pc*/
.int 0		/*V198	reset pc*/
.int 0		/*V199	reset pc*/
.int 0		/*V200	reset pc*/
.int 0		/*V201	reset pc*/
.int 0		/*V202	reset pc*/
.int 0		/*V203	reset pc*/
.int 0		/*V204	reset pc*/
.int 0		/*V205	reset pc*/
.int 0		/*V206	reset pc*/
.int 0		/*V207	reset pc*/
.int 0		/*V208	reset pc*/
.int 0		/*V209	reset pc*/
.int 0		/*V210	reset pc*/
.int 0		/*V211	reset pc*/
.int 0		/*V212	reset pc*/
.int 0		/*V213	reset pc*/
.int 0		/*V214	reset pc*/
.int 0		/*V215	reset pc*/
.int 0		/*V216	reset pc*/
.int 0		/*V217	reset pc*/
.int 0		/*V218	reset pc*/
.int 0		/*V219	reset pc*/
.int 0		/*V220	reset pc*/
.int 0		/*V221	reset pc*/
.int 0		/*V222	reset pc*/
.int 0		/*V223	reset pc*/
.int 0		/*V224	reset pc*/
.int 0		/*V225	reset pc*/
.int 0		/*V226	reset pc*/
.int 0		/*V227	reset pc*/
.int 0		/*V228	reset pc*/
.int 0		/*V229	reset pc*/
.int 0		/*V230	reset pc*/
.int 0		/*V231	reset pc*/
.int 0		/*V232	reset pc*/
.int 0		/*V233	reset pc*/
.int 0		/*V234	reset pc*/
.int 0		/*V235	reset pc*/
.int 0		/*V236	reset pc*/
.int 0		/*V237	reset pc*/
.int 0		/*V238	reset pc*/
.int 0		/*V239	reset pc*/
.int 0		/*V240	reset pc*/
.int 0		/*V241	reset pc*/
.int 0		/*V242	reset pc*/
.int 0		/*V243	reset pc*/
.int 0		/*V244	reset pc*/
.int 0		/*V245	reset pc*/
.int 0		/*V246	reset pc*/
.int 0		/*V247	reset pc*/
.int 0		/*V248	reset pc*/
.int 0		/*V249	reset pc*/
.int 0		/*V250	reset pc*/
.int 0		/*V251	reset pc*/
.int 0		/*V252	reset pc*/
.int 0		/*V253	reset pc*/
.int 0		/*V254	reset pc*/
.int 0		/*V255	reset pc*/



.text
.global _start
_start:

	moveq #5,%d0
	move.l %d0,-8(%fp)
	addq.l #1,-8(%fp)
	move.l -8(%fp),%d0
	