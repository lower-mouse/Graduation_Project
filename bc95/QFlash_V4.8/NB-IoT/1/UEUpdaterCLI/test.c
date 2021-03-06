

typedef U8 uint8
typedef U8 uint8 
typedef U16 uint16 
typedef U32 uint32 
typedef S32 int32 
typedef U16 uint16 
typedef U32 uint32
typedef BOOL8 bool
typedef uint32 secs_t;
typedef U8 uint8_t
typedef U8 PLMN_ID[3]
typedef S64 c_int64

typedef enum
{
    AT_UART_BAUD_RATE_4800 = 4800,
    AT_UART_BAUD_RATE_9600 = 9600,
    AT_UART_BAUD_RATE_57600 = 57600,
    AT_UART_BAUD_RATE_115200 = 115200,
}AT_UART_BAUD_RATE;

typedef struct
{
    AT_UART_BAUD_RATE baudrate;
    uint8 syncmode;
    uint8 stopbits;
    uint8 parity;
}at_uart_config_t;

export 0 struct TIME_MS_t
{
     uint32 Value;
} ;


export 0 struct UUID_t
{
    uint8 Value[16];
} ;

export 0 struct IMEI_t
{
    uint8 Value[8];
} ;

export 0 struct PRODUCTION_LOCK_t
{
    uint8 Value[8] ; 
} 

export 0 struct BYTE_ARRAY_t
{
    uint8 Bytes[0] ; 
} 


typedef struct
{
 uint8 Value;
} PIN_t;

export 0 PIN_t ;


typedef struct
{
 uint16 Value;
} PORT_t;
export 0 PORT_t

typedef struct
{
  uint8 Store[0];
} KEYSTORE_t 
export 0 KEYSTORE_t

typedef struct
{
    uint16 value;
} VOLTAGE_t;
export 0 VOLTAGE_t

typedef enum
{
	NB_IOT_UE_POWER_CLASS_3 = 0x3
	NB_IOT_UE_POWER_CLASS_5 = 0x5
	NB_IOT_UE_POWER_CLASS_INVALID = 0xff
} nb_iot_ue_power_class_t ;
/d/nb-iot/src/app/nbiot/stack/public/nb_iot_ue_keys.h:123

typedef enum
{
	INVALID_PDN = 0x0
	IPV4 = 0x1
	IPV6 = 0x2
	IPV4V6 = 0x3
	NON_IP = 0x5
} PDN_TYPE_e ;
/d/nb-iot/src/app/nbiot/stack/l3/nas/global/common_types.h:61

typedef enum
{
	E_PTW_VAL_0 = 0x0
	E_PTW_VAL_1 = 0x1
	E_PTW_VAL_2 = 0x2
	E_PTW_VAL_3 = 0x3
	E_PTW_VAL_4 = 0x4
	E_PTW_VAL_5 = 0x5
	E_PTW_VAL_6 = 0x6
	E_PTW_VAL_7 = 0x7
	E_PTW_VAL_8 = 0x8
	E_PTW_VAL_9 = 0x9
	E_PTW_VAL_10 = 0xa
	E_PTW_VAL_11 = 0xb
	E_PTW_VAL_12 = 0xc
	E_PTW_VAL_13 = 0xd
	E_PTW_VAL_14 = 0xe
	E_PTW_VAL_15 = 0xf
} PAGING_TIME_WINDOW_LEN_e ;
/d/nb-iot/src/app/nbiot/stack/l3/nas/global/mn_interface_types.h:116

typedef enum
{
	E_DRX_CYCLE_PARAM_2 = 0x2
	E_DRX_CYCLE_PARAM_3 = 0x3
	E_DRX_CYCLE_PARAM_5 = 0x5
	E_DRX_CYCLE_PARAM_9 = 0x9
	E_DRX_CYCLE_PARAM_10 = 0xa
	E_DRX_CYCLE_PARAM_11 = 0xb
	E_DRX_CYCLE_PARAM_12 = 0xc
	E_DRX_CYCLE_PARAM_13 = 0xd
	E_DRX_CYCLE_PARAM_14 = 0xe
	E_DRX_CYCLE_PARAM_15 = 0xf
	NUM_E_DRX_CYCLE_PARAM = 0x10
} eDRX_CYCLE_PARAM_e ;
/d/nb-iot/src/app/nbiot/stack/l3/nas/global/mn_interface_types.h:95

typedef enum
{
	EPS_INTG_ALG_EIA0_NULL = 0x0
	EPS_INTG_ALG_128_EIA1 = 0x1
	EPS_INTG_ALG_128_EIA2 = 0x2
	EPS_INTG_ALG_EIA3 = 0x3
	EPS_INTG_ALG_EIA4 = 0x4
	EPS_INTG_ALG_EIA5 = 0x5
	EPS_INTG_ALG_EIA6 = 0x6
	EPS_INTG_ALG_EIA7 = 0x7
} INTG_ALG_e ;
/d/nb-iot/src/app/nbiot/stack/l3/nas/global/emm_interface_types.h:388

typedef enum
{
	EPS_CIPH_ALG_EEA0_NULL = 0x0
	EPS_CIPH_ALG_128_EEA1 = 0x1
	EPS_CIPH_ALG_128_EEA2 = 0x2
	EPS_CIPH_ALG_EEA3 = 0x3
	EPS_CIPH_ALG_EEA4 = 0x4
	EPS_CIPH_ALG_EEA5 = 0x5
	EPS_CIPH_ALG_EEA6 = 0x6
	EPS_CIPH_ALG_EEA7 = 0x7
} CIPH_ALG_e ;
/d/nb-iot/src/app/nbiot/stack/l3/nas/global/emm_interface_types.h:403

typedef enum
{
	RRC_EDRX_UEID_TYPE_IMSI = 0x0
	RRC_EDRX_UEID_TYPE_CRC32_ITU_T = 0x1
	RRC_EDRX_UEID_TYPE_CRC32_POLY_LSB = 0x2
	RRC_EDRX_UEID_TYPE_CRC32_POLY_INIT_LSB = 0x3
	RRC_EDRX_UEID_TYPE_CRC32_POLY_MSB = 0x4
	RRC_EDRX_UEID_TYPE_CRC32_POLY_INIT_MSB = 0x5
	RRC_EDRX_UEID_TYPE_CRC32_POLY_INIT_FINAL_XOR_MSB = 0x6
} rrc_edrx_ueid_type_t ;
/d/nb-iot/src/app/nbiot/stack/l3/rrc/public/rrc_shared_types.h:64

typedef enum
{
	RRC_EDRX_UEID_MTMSI_B0_FIRST_INSEQ = 0x0
	RRC_EDRX_UEID_MTMSI_B31_FIRST_INSEQ = 0x1
	RRC_EDRX_UEID_MTMSI_B31_OCTET1_MSB = 0x2
	RRC_EDRX_UEID_MTMSI_B0_OCTET1_MSB = 0x3
} rrc_edrx_ueid_bo_t ;
/d/nb-iot/src/app/nbiot/stack/l3/rrc/public/rrc_shared_types.h:77

typedef struct
{
	uint8 num;
	nb_iot_ue_band_map_entry_t band_map_entry[14];
} band_map_power_class_t ;
# /d/nb-iot/src/app/nbiot/stack/public/nb_iot_ue_keys.h:137

typedef struct
{
	uint8 band;
	nb_iot_ue_power_class_t power_class;
} nb_iot_ue_band_map_entry_t ;
# /d/nb-iot/src/app/nbiot/stack/public/nb_iot_ue_keys.h:130

typedef struct
{
	E_EQUIV_PLMN_LIST_s eplmn_list;
	IMSI_s imsi;
} EPLMN_LIST_STORE_INFO_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:179

typedef struct
{
	uint16 list_count;
	PLMN_ID list[16];
} E_EQUIV_PLMN_LIST_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/emmsm/inc/emm_plmn_types.h:37

typedef struct
{
	uint8 mnc_length;
	uint16 len;
	uint8 imsi[9];
} IMSI_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/emmsm/inc/emm_identity_types.h:56

typedef struct
{
	bool enable_fast_scan;
	bool early_exit_slow_scan;
} ll1_channel_scan_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2892

typedef struct
{
	uint16 sss_resync_max_timing_err;
	uint8 rtc_clock_err;
	uint8 pad;
} ll1_lpm_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2886

typedef struct
{
	uint8 ncell_invisible_threshold;
} ll1_ncell_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2898

typedef struct
{
	uint32 fast_search_period;
	uint32 midd_search_period;
	uint32 slow_search_period;
} ll1_npbch_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2866

typedef struct
{
	uint32 slow_search_period;
	uint32 fast_search_period;
	uint16 cfo_tolerance;
	uint16 cfo_offsets;
} ll1_npss_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2844

typedef struct
{
	uint32 resync_gap;
	uint16 resync_max_timing_err;
	uint16 periodic_resync_max_timing_err;
	uint8 resync_avg_window_normal_coverage;
	uint8 resync_avg_window_deep_coverage;
	uint8 pad[2];
} ll1_nsss_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2859

typedef struct
{
	uint16 npdcch_rep_ecl_0;
	uint16 npdcch_rep_ecl_1;
	uint16 npdcch_rep_ecl_2;
} ll1_paging_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2911

typedef struct
{
	uint8 max_rep_si_comb;
	uint8 max_rep_sib1_comb;
} ll1_sofcomb_params_t ;
# /d/nb-iot/src/app/nbiot/stack/ll1/public/ll1_messages.h:2874

typedef struct
{
	bool emm_reg_w_o_pdn;
	PDN_TYPE_e pdn_type;
	uint8 apn_len;
	uint8 apn[100];
	uint8 pad[3];
} nas_kv_context_config_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:54

typedef struct
{
	bool e_drx_is_supported;
	PAGING_TIME_WINDOW_LEN_e paging_time_window;
	eDRX_CYCLE_PARAM_e edrx_cycle;
} nas_kv_edrx_ptw_config_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:200

typedef struct
{
	bool nslpi_configured;
	bool nslpi_override_configured;
	bool excpt_data_report_configured;
	bool fast_first_hpplmn_configured;
	bool min_periodic_search_time_available;
	uint8 min_periodic_search_time;
	bool t3245_behaviour;
} nas_kv_nas_config_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:92

typedef struct
{
	uint8 k[16];
	uint8 opc[16];
} nas_kv_ocsim_auth_params_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:149

typedef struct
{
	USIM_OC_EF_s ocsim_ef;
} nas_kv_ocsim_ef_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:128

typedef struct
{
	USIM_EF_ICCID_s ef_iccid;
	USIM_IMSI_s ef_imsi;
	uint8 ef_ad[4];
	uint8 ef_acc[2];
	uint8 ef_hpplmn;
	bool ef_lrplmnsi;
} USIM_OC_EF_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/sim/inc/sim.h:325

typedef struct
{
	uint8 iccid[10];
} USIM_EF_ICCID_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/sim/inc/sim.h:277

typedef struct
{
	uint16 len;
	uint8 imsi[8];
} USIM_IMSI_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/global/interface/emm_usim_if.h:125

typedef struct
{
	USIM_OC_SMS_EF_s ocsim_sms_ef;
} nas_kv_ocsim_sms_ef_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:138

typedef struct
{
	uint8 last_used_tp_mr;
	uint8 tp_serv_cent_addr[12];
} USIM_OC_SMS_EF_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/sim/inc/sim.h:332

typedef struct
{
	bool has_synchronized;
	uint8 req_psm_active_timer;
	uint8 req_ext_periodic_timer;
	PAGING_TIME_WINDOW_LEN_e paging_time_window;
	eDRX_CYCLE_PARAM_e edrx_cycle;
} nas_kv_psm_edrx_default_value_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:214

typedef struct
{
	bool enable_ext_sim;
	bool enable_eia0;
	bool enable_eea0;
	bool enable_eia1;
	bool enable_eea1;
	bool enable_eia2;
	bool enable_eea2;
	bool enable_eia3;
	bool enable_eea3;
} nas_kv_sim_config_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:41

typedef struct
{
	bool enable_sim_presence_detection;
} nas_kv_sim_presence_enable_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:171

typedef struct
{
	uint32 backoff_timer_remainder;
	bool backoff_for_low_priority;
	uint32 t3245_remainder;
} nas_kv_timer_remainders_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:162

typedef struct
{
	USIM_IMSI_s imsi;
	USIM_EPS_LOCI_s eps_loci;
	NVRAM_EPS_NAS_SECURITY_CONTEXT_s eps_nas_security_context;
} nas_kv_usim_eps_params_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:76

typedef struct
{
	uint8 guti[12];
	uint8 tai[5];
	uint8 update_status;
} USIM_EPS_LOCI_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/global/interface/emm_usim_if.h:89

typedef struct
{
	uint8 e_ksi;
	uint8 k_asme[32];
	uint32 uplink_nas_count;
	uint32 dnlink_nas_count;
	INTG_ALG_e intg_algorithm;
	CIPH_ALG_e ciph_algorithm;
} NVRAM_EPS_NAS_SECURITY_CONTEXT_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:69

typedef struct
{
	bool psm_supported;
	uint8 req_psm_active_timer;
	bool ext_timer_support;
	uint8 req_ext_periodic_timer;
	bool emm_reg_w_o_pdn_supported;
} NAS_UE_CAPABILITY_CONFIG_s ;
# /d/nb-iot/src/app/nbiot/stack/l3/nas/private/nas_keys.h:106

typedef struct
{
	uint8 num_supported_bands;
	uint8 band_ids[14];
} nb_iot_ue_kv_supported_bands_t ;
# /d/nb-iot/src/app/nbiot/stack/public/nb_iot_ue_keys.h:36

typedef struct
{
	bool autostart_enabled;
	uint8 default_plmn[3];
} nb_iot_ue_nvconfig_autostart_config_t ;
# /d/nb-iot/src/app/nbiot/stack/public/nb_iot_ue_keys.h:55

typedef struct
{
	bool bip_enabled;
} nb_iot_ue_nvconfig_bip_config_t ;
# /d/nb-iot/src/app/nbiot/stack/public/nb_iot_ue_keys.h:104

typedef struct
{
	uint16 first_full_search_interval;
	uint16 full_search_interval_mult;
	uint16 max_full_search_interval;
	uint16 num_full_fast_searches;
	uint16 first_stored_search_interval;
	uint16 stored_search_interval_mult;
	uint16 max_stored_search_interval;
	uint16 band_scan_puncture_rate;
	c_int64 fast_search_threshold;
	c_int64 slow_search_threshold;
} nb_iot_ue_nvconfig_plmn_search_config_t ;
# /d/nb-iot/src/app/nbiot/stack/public/nb_iot_ue_keys.h:94

typedef struct
{
	rrc_edrx_ueid_type_t edrx_ueid_type;
	rrc_edrx_ueid_bo_t edrx_ueid_bo;
} rrc_kv_crc_cfg_type_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/rrc/private/rrc_keys.h:25

typedef struct
{
	uint32 earfcn;
	bool deep_coverage;
	uint8 num_plmns;
	rrc_kv_plmn_id_t plmns[6];
} rrc_kv_last_earfcn_used_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/rrc/private/rrc_keys.h:58

typedef struct
{
	uint8 mcc1;
	uint8 mcc2;
	uint8 mcc3;
	uint8 mnc1;
	uint8 mnc2;
	uint8 mnc3;
	uint8 mnc_digits;
} rrc_kv_plmn_id_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/rrc/private/rrc_keys.h:50

typedef struct
{
	uint8 number_item;
	uint8 index;
	rrc_kv_plmn_earfcn_mapping_item_t item[3];
} rrc_kv_plmn_earfcn_mapping_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/rrc/private/rrc_keys.h:65

typedef struct
{
	rrc_kv_plmn_id_t plmn;
	uint8 number_earfcn;
	uint8 index;
	uint32 earfcns[5];
} rrc_kv_plmn_earfcn_mapping_item_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/rrc/private/rrc_keys.h:66

typedef struct
{
	uint32 mcc1 : 4;
	uint32 mcc2 : 4;
	uint32 mcc3 : 4;
	uint32 mnc1 : 4;
	uint32 mnc2 : 4;
	uint32 mnc3 : 4;
	uint32 mcc_received : 1;
	uint32 mnc_digits : 2;
	uint32 operator_reserved : 1;
	uint32 att_without_pdn_conn : 1;
} rrc_db_plmn_id_info_t ;
# /d/nb-iot/src/app/nbiot/stack/l3/rrc/private/rrc_db_types.h:205




typedef struct 
{
 bool Value;
}NAS_COMBINE_ATTACH_t;
typedef struct 
{
 bool Value;
}NAS_T3245_USED_t;
typedef struct 
{
 bool Value;
}NAS_LOW_PRIORITY_SUPPORT_t;
typedef struct 
{
 uint8 Value;
}UE_SIB_ATTEMPT_COUNTER_t;
typedef struct 
{
 bool Value;
}RRC_RESELECTION_CONFIG_t;


typedef struct 
{
 secs_t Value;
}UE_DATA_INACTIVITY_TIMEOUT_t;

typedef struct 
{
 uint8 Value;
}UE_NCELL_SIB_ATTEMPT_COUNTER_t;

typedef struct 
{
 bool Value;
}LL1_SEARCH_SPEED_OVERRIDE_t;
typedef struct 
{
 bool Value;
}LL1_AUTO_LPM_DISABLED_t;
typedef struct 
{
 bool Value;
}LL1_CR_0354_0338_SCRAMBLING_CONFIG_t;
typedef struct 
{
 bool Value;
}LL1_CR_0859_SI_AVOID_CONFIG_t;
typedef struct 
{
 uint32 Value;
}LL1_RF_RETRIM_PERIOD_t;
typedef struct 
{
 bool Value;
}LL1_PDDCH_ORDER_PREAMBLE_OFFSET_ADD_t;
typedef struct 
{
 uint32 Value;
}LL1_MIN_ACCEPTABLE_SSS_POWER_RATIO_t;


typedef struct 
{
 uint8 Value;
}RADIO_PRODUCTION_LOCK_KEY_t;



typedef struct 
{
 uint8 Value;
}UE_MULTITONE_ENABLED_t;
typedef struct 
{
 uint8 Value;
}UE_CONFIGURED_REL_VER_t;
typedef struct 
{
 uint8 Value;
}CLOCK_ROUTE_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}SPI_CLK_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}SPI_MOSI_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}SPI_MISO_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}SPI_MEM_CS_PIN_KEY_t;


typedef struct 
{
 uint8 Value;
}UICC_BEHAVIOUR_KEY_t;
typedef struct 
{
 uint8 Value;
}UICC_IO_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}UICC_RST_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}UICC_CLK_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}UICC_PWR_PIN_KEY_t;
typedef struct 
{
 uint8 Value;
}UICC_PWR_PIN2_KEY_t;
typedef struct 
{
 uint8 Value;
}UICC_DET_PIN_KEY_t;


typedef struct 
{
 uint8 Value;
}ACTIVITY_INDICATION_PIN_KEY_t;

typedef struct 
{
 uint8 Value;
}LOG_UART_TX_PIN_t;
typedef struct 
{
 bool Value;
}LOG_NXLOG_CONFIG_KEY_t;

typedef struct 
{
 uint8_t Value;
}OTA_UPDATE_STATE_KEY_t;
typedef struct 
{
 uint8_t Value;
}OTA_UPDATE_RESULT_KEY_t;

typedef struct 
{
 uint8 Value;
}OTA_UPDATE_MODE_KEY_t;
typedef struct 
{
 uint16 Value;
}TUP_CDP_PORT_t;



export 0  NAS_COMBINE_ATTACH_t 
export 0  NAS_T3245_USED_t 
export 0  NAS_LOW_PRIORITY_SUPPORT_t 
export 0  UE_SIB_ATTEMPT_COUNTER_t 
export 0  RRC_RESELECTION_CONFIG_t 

export 0  UE_NCELL_SIB_ATTEMPT_COUNTER_t 
export 0  UE_DATA_INACTIVITY_TIMEOUT_t 
export 0  LL1_SEARCH_SPEED_OVERRIDE_t 
export 0  LL1_AUTO_LPM_DISABLED_t 

export 0  LL1_RF_RETRIM_PERIOD_t 
export 0  LL1_PDDCH_ORDER_PREAMBLE_OFFSET_ADD_t 
export 0  LL1_MIN_ACCEPTABLE_SSS_POWER_RATIO_t 

export 0  RADIO_PRODUCTION_LOCK_KEY_t 

export 0  UE_MULTITONE_ENABLED_t 
export 0  UE_CONFIGURED_REL_VER_t 
export 0  CLOCK_ROUTE_PIN_KEY_t 
export 0  SPI_CLK_PIN_KEY_t 
export 0  SPI_MOSI_PIN_KEY_t 
export 0  SPI_MISO_PIN_KEY_t 
export 0  SPI_MEM_CS_PIN_KEY_t 
export 0  UICC_BEHAVIOUR_KEY_t 
export 0  UICC_IO_PIN_KEY_t 
export 0  UICC_RST_PIN_KEY_t 
export 0  UICC_CLK_PIN_KEY_t 
export 0  UICC_PWR_PIN_KEY_t 
export 0  UICC_PWR_PIN2_KEY_t 
export 0  UICC_DET_PIN_KEY_t 
export 0  ACTIVITY_INDICATION_PIN_KEY_t 
export 0  LOG_UART_TX_PIN_t 
export 0  LOG_NXLOG_CONFIG_KEY_t 
export 0  OTA_UPDATE_STATE_KEY_t 
export 0  OTA_UPDATE_RESULT_KEY_t 
export 0  OTA_UPDATE_MODE_KEY_t 
export 0  TUP_CDP_PORT_t 





export 0  nas_kv_sim_config_t 
export 0  nas_kv_context_config_t 
export 0  nas_kv_usim_eps_params_t 
export 0  nas_kv_nas_config_t 
export 0  NAS_UE_CAPABILITY_CONFIG_s 
export 0  nas_kv_ocsim_ef_t 
export 0  nas_kv_ocsim_sms_ef_t 
export 0  nas_kv_ocsim_auth_params_t 
export 0  nas_kv_timer_remainders_t 
export 0  nas_kv_sim_presence_enable_t 
export 0  EPLMN_LIST_STORE_INFO_s 
export 0  nas_kv_edrx_ptw_config_t 
export 0  nas_kv_psm_edrx_default_value_t 
export 0  nb_iot_ue_nvconfig_plmn_search_config_t 
export 0  nb_iot_ue_nvconfig_autostart_config_t 
export 0  nb_iot_ue_nvconfig_bip_config_t 
export 0  rrc_kv_crc_cfg_type_t 
export 0  rrc_kv_last_earfcn_used_t 
export 0  rrc_kv_plmn_earfcn_mapping_t 
export 0  nb_iot_ue_kv_supported_bands_t 
export 0  ll1_paging_params_t 
export 0  ll1_npss_params_t 
export 0  ll1_nsss_params_t 
export 0  ll1_npbch_params_t 
export 0  ll1_lpm_params_t 
export 0  ll1_ncell_params_t 
export 0  ll1_channel_scan_params_t 
export 0  ll1_sofcomb_params_t 
export 0  band_map_power_class_t 
#export 0  pin_map_buf 
#export 0  uicc_pwr_class_c 
#export 0  uicc_pwr_class_b 
#export 0  uicc_pwr_class_a 
export 0  at_uart_config_t 
#export 0  level 
#export 0  local_r_addr 
#export 0  local_w_addr 
#export 0  str_token 
#export 0  ip_addr_t 




