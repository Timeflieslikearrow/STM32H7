/**********************************************************************************************************************
 *  GENIS KOREA SOFTWARE
 *  -------------------------------------------------------------------------------------------------------------------
 *  Copyright (c) 2019-2021 GENIS KOREA Inc.                                                 All rights reserved.
 *
 *                This software is copyright protected and proprietary to GENIS KOREA.
 *                Restricted Distribution:
 *                  Not to be distributed This source code to anyone without the approval from GENIS KOREA.
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Filename: Csm_Dbcs.c
 *  Created: Apr 3, 2024
 *  Author: sbkang
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Csm_Dbcs.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
//#pragma pack(push,1)
//#pragma pack(pop)
/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

#define BIT(VALUE, POS) ((VALUE & (1 << POS)) >> POS)
#define BIT_VALUE(VAR, POS) (VAR & (1 << POS))
#define BIT_SET(VAR, POS) (VAR |= (1 << POS))
#define BIT_CLEAR(VAR, POS) (VAR &= ((1 << POS) ^ 255))

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

void can_csm_secc_from_map(can_csm_secc_t *secc_bus, uint16 *in)
{
  secc_bus->SeccInformation.SeccReady = in[0];
  secc_bus->SeccInformation.SeccStatusCode = in[1];
  secc_bus->SeccInformation.SeccErrorCode = in[2];
  secc_bus->SeccInformation.SeccSWVersion = in[3];
  secc_bus->SeccInformation.SeccProtocol = in[4];
  secc_bus->SeccInformation.PwmDutyCycle = in[5];
  secc_bus->SeccInformation.PwmVoltage = in[6];

  secc_bus->EvChargingService.SelectedPaymentOption = in[14] & 0x01;
  secc_bus->EvChargingService.Iso20ControlMode = (in[14] >> 2) & 0x03;
  secc_bus->EvChargingService.BPT_ChannelSelection = (in[14] >> 4) & 0x03;
  secc_bus->EvChargingService.RequestedEnergyTransferType = in[15];
  secc_bus->EvChargingService.MaxSupportPoint = in[16];
  secc_bus->EvChargingService.DepartureTime = in[17] | (in[18] << 16);

  secc_bus->EvAcPresentActivePower.EVPresentActivePower = in[20];
  secc_bus->EvAcPresentActivePower.EVPresentActivePower_L2 = in[21];
  secc_bus->EvAcPresentActivePower.EVPresentActivePower_L3 = in[22];
  secc_bus->EvAcPresentReactivePower.EVPresentReactivePower = in[23];
  secc_bus->EvAcPresentReactivePower.EVPresentReactivePower_L2 = in[24];
  secc_bus->EvAcPresentReactivePower.EVPresentReactivePower_L3 = in[25];

  secc_bus->EvAcMaximumChargePower.EVMaximumChargePower = in[26];
  secc_bus->EvAcMaximumChargePower.EVMaximumChargePower_L2 = in[27];
  secc_bus->EvAcMaximumChargePower.EVMaximumChargePower_L3 = in[28];
  secc_bus->EvAcMinimumChargePower.EVMinimumChargePower = in[29];
  secc_bus->EvAcMinimumChargePower.EVMinimumChargePower_L2 = in[30];
  secc_bus->EvAcMinimumChargePower.EVMinimumChargePower_L3 = in[31];

  secc_bus->EvTargetEnergy.EVTargetEnergyRequest = in[32];
  secc_bus->EvEnergyRequestLimit.EVMaximumEnergyRequest = in[33];
  secc_bus->EvEnergyRequestLimit.EVMinimumEnergyRequest = in[34];
  secc_bus->EvEnergyRequestLimit.EVMaximumV2XEnergyRequest = in[36];
  secc_bus->EvEnergyRequestLimit.EVMinimumV2XEnergyRequest = in[37];

  secc_bus->EvDisplayPresentStatus.PresentSoc = in[39];
  secc_bus->EvDisplayMinimumSoc.MinimumSoc = in[40];
  secc_bus->EvDisplayTargetSoc.TargetSoc = in[41];
  secc_bus->EvDisplayMaximumSoc.MaximumSoc = in[42];

  secc_bus->EvDisplayMinimumSoc.RemainingTime = in[43] | (in[44] << 16);
  secc_bus->EvDisplayTargetSoc.RemainingTime = in[45] | (in[46] << 16);
  secc_bus->EvDisplayMaximumSoc.RemainingTime = in[47] | (in[48] << 16);

  secc_bus->EvDisplayPresentStatus.ChargingComplete = in[49];
  secc_bus->EvDisplayPresentStatus.BatteryEnergyCapacity = in[50];
  secc_bus->EvDisplayPresentStatus.InletHot = in[51];

  secc_bus->EvAcMaximumDischargePower.EVMaximumDischargePower = in[54];
  secc_bus->EvAcMaximumDischargePower.EVMaximumDischargePower_L2 = in[55];
  secc_bus->EvAcMaximumDischargePower.EVMaximumDischargePower_L3 = in[56];

  secc_bus->EvAcMinimumDischargePower.EVMinimumDischargePower = in[57];
  secc_bus->EvAcMinimumDischargePower.EVMinimumDischargePower_L2 = in[58];
  secc_bus->EvAcMinimumDischargePower.EVMinimumDischargePower_L3 = in[59];

  memcpy(&secc_bus->EvEvccId, &in[60], 8);
}
uint8 can_csm_secc_to_map(const can_csm_secc_t *secc_bus, uint16 *out)
{
  out[0] = secc_bus->SeccInformation.SeccReady;
  out[1] = secc_bus->SeccInformation.SeccStatusCode;
  out[2] = secc_bus->SeccInformation.SeccErrorCode;
  out[3] = secc_bus->SeccInformation.SeccSWVersion;

  out[4] = secc_bus->SeccInformation.SeccProtocol;
  out[5] = secc_bus->SeccInformation.PwmDutyCycle;
  out[6] = secc_bus->SeccInformation.PwmVoltage;

  out[14] = secc_bus->EvChargingService.SelectedPaymentOption | (secc_bus->EvChargingService.Iso20ControlMode << 2)
      | (secc_bus->EvChargingService.BPT_ChannelSelection << 4);

  out[15] = secc_bus->EvChargingService.RequestedEnergyTransferType;
  out[16] = secc_bus->EvChargingService.MaxSupportPoint;
  out[17] = (secc_bus->EvChargingService.DepartureTime & 0xFFFF);
  out[18] = (secc_bus->EvChargingService.DepartureTime >> 16) & 0xFFFF;

  out[20] = secc_bus->EvAcPresentActivePower.EVPresentActivePower;
  out[21] = secc_bus->EvAcPresentActivePower.EVPresentActivePower_L2;
  out[22] = secc_bus->EvAcPresentActivePower.EVPresentActivePower_L3;

  out[23] = secc_bus->EvAcPresentReactivePower.EVPresentReactivePower;
  out[24] = secc_bus->EvAcPresentReactivePower.EVPresentReactivePower_L2;
  out[25] = secc_bus->EvAcPresentReactivePower.EVPresentReactivePower_L3;

  out[26] = secc_bus->EvAcMaximumChargePower.EVMaximumChargePower;
  out[27] = secc_bus->EvAcMaximumChargePower.EVMaximumChargePower_L2;
  out[28] = secc_bus->EvAcMaximumChargePower.EVMaximumChargePower_L3;

  out[29] = secc_bus->EvAcMinimumChargePower.EVMinimumChargePower;
  out[30] = secc_bus->EvAcMinimumChargePower.EVMinimumChargePower_L2;
  out[31] = secc_bus->EvAcMinimumChargePower.EVMinimumChargePower_L3;

  out[32] = secc_bus->EvTargetEnergy.EVTargetEnergyRequest;

  out[33] = secc_bus->EvEnergyRequestLimit.EVMaximumEnergyRequest;
  out[34] = secc_bus->EvEnergyRequestLimit.EVMinimumEnergyRequest;
  out[36] = secc_bus->EvEnergyRequestLimit.EVMaximumV2XEnergyRequest;
  out[37] = secc_bus->EvEnergyRequestLimit.EVMinimumV2XEnergyRequest;

  out[39] = secc_bus->EvDisplayPresentStatus.PresentSoc;
  out[40] = secc_bus->EvDisplayMinimumSoc.MinimumSoc;
  out[41] = secc_bus->EvDisplayTargetSoc.TargetSoc;
  out[42] = secc_bus->EvDisplayMaximumSoc.MaximumSoc;

  out[43] = (secc_bus->EvDisplayMinimumSoc.RemainingTime & 0xFFFF);
  out[44] = (secc_bus->EvDisplayMinimumSoc.RemainingTime >> 16) & 0xFFFF;

  out[45] = (secc_bus->EvDisplayTargetSoc.RemainingTime & 0xFFFF);
  out[46] = (secc_bus->EvDisplayTargetSoc.RemainingTime >> 16) & 0xFFFF;

  out[47] = (secc_bus->EvDisplayMaximumSoc.RemainingTime & 0xFFFF);
  out[48] = (secc_bus->EvDisplayMaximumSoc.RemainingTime >> 16) & 0xFFFF;

  out[49] = secc_bus->EvDisplayPresentStatus.ChargingComplete;
  out[50] = secc_bus->EvDisplayPresentStatus.BatteryEnergyCapacity;
  out[51] = secc_bus->EvDisplayPresentStatus.InletHot;

  out[54] = secc_bus->EvAcMaximumDischargePower.EVMaximumDischargePower;
  out[55] = secc_bus->EvAcMaximumDischargePower.EVMaximumDischargePower_L2;
  out[56] = secc_bus->EvAcMaximumDischargePower.EVMaximumDischargePower_L3;

  out[57] = secc_bus->EvAcMinimumDischargePower.EVMinimumDischargePower;
  out[58] = secc_bus->EvAcMinimumDischargePower.EVMinimumDischargePower_L2;
  out[59] = secc_bus->EvAcMinimumDischargePower.EVMinimumDischargePower_L3;

  memcpy(&out[60], secc_bus->EvEvccId.EvccId, sizeof(secc_bus->EvEvccId.EvccId));

  return (60 + 10);
}

uint8 can_csm_charger_to_map(const can_csm_charger_t *charger_bus, uint16 *out)
{
  // Charger Send Message
  BIT_CLEAR(out[0], 0);
  charger_bus->ChargerStatus.ChargingStatus ? BIT_SET(out[0], 1) : BIT_CLEAR(out[0], 1);
  charger_bus->ChargerStatus.ChargingStopControl ? BIT_SET(out[0], 2) : BIT_CLEAR(out[0], 2);
  charger_bus->ChargerStatus.EVSEMalfunction ? BIT_SET(out[0], 3) : BIT_CLEAR(out[0], 3);
  charger_bus->ChargerStatus.EmergencyShutdown ? BIT_SET(out[0], 4) : BIT_CLEAR(out[0], 4);
  charger_bus->ChargerStatus.AuthFinished ? BIT_SET(out[0], 5) : BIT_CLEAR(out[0], 5);
  charger_bus->ChargerStatus.ChargeParameterFinished ? BIT_SET(out[0], 6) : BIT_CLEAR(out[0], 6);
  charger_bus->ChargerStatus.DcCableIsolationChecked ? BIT_SET(out[0], 7) : BIT_CLEAR(out[0], 7);

  out[5] = charger_bus->ChargerStatus.ChargingEnergyTransferType | charger_bus->ChargerStatus.BptSupported << 4
      | charger_bus->ChargerStatus.ChargingPriority << 6;

  out[6] = charger_bus->ChargerStatus.IdentificationOptionType;
  out[7] = charger_bus->ChargerStatus.Iso20ControlMode;
  
  out[8] = charger_bus->ChargerStatus.VasEnable;

  out[9] = (uint16) (charger_bus->ChargerChargingService.DepartureTime & 0xFFFF);
  out[10] = (uint16) ((charger_bus->ChargerChargingService.DepartureTime >> 16) & 0xFFFF);
  out[11] = charger_bus->ChargerChargingService.MinimumSoc;
  out[12] = charger_bus->ChargerChargingService.TargetSoc;

  out[14] = charger_bus->ChargerChargingVariable.MeteringValue & 0xFFFF;
  out[15] = (charger_bus->ChargerChargingVariable.MeteringValue >> 16) & 0xFFFF;
  out[16] = charger_bus->ChargerAcChargeParam.EVSETargetFrequency;
  out[17] = charger_bus->ChargerAcPresentActivePower.EVSEPresentActivePower;
  out[18] = charger_bus->ChargerAcPresentActivePower.EVSEPresentActivePower_L2;
  out[19] = charger_bus->ChargerAcPresentActivePower.EVSEPresentActivePower_L3;

  out[20] = charger_bus->ChargerAcTargetActivePower.EVSETargetActivePower;
  out[21] = charger_bus->ChargerAcTargetActivePower.EVSETargetActivePower_L2;
  out[22] = charger_bus->ChargerAcTargetActivePower.EVSETargetActivePower_L3;

  out[23] = charger_bus->ChargerAcTargetReactivePower.EVSETargetReactivePower;
  out[24] = charger_bus->ChargerAcTargetReactivePower.EVSETargetReactivePower_L2;
  out[25] = charger_bus->ChargerAcTargetReactivePower.EVSETargetReactivePower_L3;

  out[26] = charger_bus->ChargerAcChargeParam.EVSEMaxCurrent;
  out[27] = charger_bus->ChargerAcChargeParam.EVSENominalVoltage;
  out[28] = charger_bus->ChargerAcChargeParam.EVSENominalFrequency;

  out[29] = charger_bus->ChargerAcMaximumChargePower.EVSEMaximumChargePower;
  out[30] = charger_bus->ChargerAcMaximumChargePower.EVSEMaximumChargePower_L2;
  out[31] = charger_bus->ChargerAcMaximumChargePower.EVSEMaximumChargePower_L3;

  out[32] = charger_bus->ChargerAcMinimumChargePower.EVSEMinimumChargePower;
  out[33] = charger_bus->ChargerAcMinimumChargePower.EVSEMinimumChargePower_L2;
  out[34] = charger_bus->ChargerAcMinimumChargePower.EVSEMinimumChargePower_L3;

  out[35] = charger_bus->ChargerAcChargingSafety.MaximumPowerAsymmetry;
  out[36] = charger_bus->ChargerAcChargingSafety.EVSEPowerRampLimitation;

  out[39] = charger_bus->ChargerChargingService.GenerateMode;
  out[40] = charger_bus->ChargerChargingService.BPTChannel;
  out[41] = charger_bus->ChargerChargingService.GridCodeIslandingDetectionMethod;

  out[42] = charger_bus->ChargerAcMaximumDischargePower.EVSEMaximumDischargePower;
  out[43] = charger_bus->ChargerAcMaximumDischargePower.EVSEMaximumDischargePower_L2;
  out[44] = charger_bus->ChargerAcMaximumDischargePower.EVSEMaximumDischargePower_L3;

  out[45] = charger_bus->ChargerAcMinimumDischargePower.EVSEMinimumDischargePower;
  out[46] = charger_bus->ChargerAcMinimumDischargePower.EVSEMinimumDischargePower_L2;
  out[47] = charger_bus->ChargerAcMinimumDischargePower.EVSEMinimumDischargePower_L3;

  out[48] = charger_bus->ChargerAcFrtLvrt.LVRTMomentaryCessationEnable;
  out[49] = charger_bus->ChargerAcFrtLvrt.LVRTMomentaryCessationVoltage;
  out[50] = charger_bus->ChargerAcFrtLvrt.LVRTMomentaryCessationRecoveryV;

  out[51] = charger_bus->ChargerAcFrtHvrt.HVRTMomentaryCessationEnable;
  out[52] = charger_bus->ChargerAcFrtHvrt.HVRTMomentaryCessationVoltage;
  out[53] = charger_bus->ChargerAcFrtHvrt.HVRTMomentaryCessationRecoveryV;

  out[54] = charger_bus->ChargerAcFrtReactivePower.ReactivePowerResponseTimeEnable;
  out[55] = charger_bus->ChargerAcFrtReactivePower.ReactivePowerControlResponse;

  return 56;
}

void can_csm_charger_from_map(can_csm_charger_t *charger_bus, uint16 *in)
{
  charger_bus->ChargerStatus.ChargingStatus = (in[0] >> 1) & 0x01;
  charger_bus->ChargerStatus.ChargingStopControl = (in[0] >> 2) & 0b111;
  charger_bus->ChargerStatus.AuthFinished = (in[0] >> 5) & 0x01;
  charger_bus->ChargerStatus.ChargeParameterFinished = (in[0] >> 6) & 0x01;

  charger_bus->ChargerStatus.ChargingEnergyTransferType = in[5] & 0xF;
  charger_bus->ChargerStatus.BptSupported = (in[5] >> 4) & 0x3;
  charger_bus->ChargerStatus.ChargingPriority = (in[5] >> 6) & 0x3;

  charger_bus->ChargerStatus.IdentificationOptionType = in[6];
  charger_bus->ChargerStatus.Iso20ControlMode = (in[7] >> 0) & 0b11;
  charger_bus->ChargerStatus.VasEnable = in[8];

  charger_bus->ChargerChargingService.DepartureTime = in[9] | (in[10] << 16);
  charger_bus->ChargerChargingService.MinimumSoc = in[11];
  charger_bus->ChargerChargingService.TargetSoc = in[12];

  charger_bus->ChargerChargingVariable.MeteringValue = in[14] | (in[15] << 16);
  charger_bus->ChargerAcChargeParam.EVSETargetFrequency = in[16];

  charger_bus->ChargerAcPresentActivePower.EVSEPresentActivePower = in[17];
  charger_bus->ChargerAcPresentActivePower.EVSEPresentActivePower_L2 = in[18];
  charger_bus->ChargerAcPresentActivePower.EVSEPresentActivePower_L3 = in[19];

  charger_bus->ChargerAcTargetActivePower.EVSETargetActivePower = in[20];
  charger_bus->ChargerAcTargetActivePower.EVSETargetActivePower_L2 = in[21];
  charger_bus->ChargerAcTargetActivePower.EVSETargetActivePower_L3 = in[22];

  charger_bus->ChargerAcTargetReactivePower.EVSETargetReactivePower = in[23];
  charger_bus->ChargerAcTargetReactivePower.EVSETargetReactivePower_L2 = in[24];
  charger_bus->ChargerAcTargetReactivePower.EVSETargetReactivePower_L3 = in[25];

  charger_bus->ChargerAcChargeParam.EVSEMaxCurrent = in[26];
  charger_bus->ChargerAcChargeParam.EVSENominalVoltage = in[27];
  charger_bus->ChargerAcChargeParam.EVSETargetFrequency = in[28];

  charger_bus->ChargerAcMaximumChargePower.EVSEMaximumChargePower = in[29];
  charger_bus->ChargerAcMaximumChargePower.EVSEMaximumChargePower_L2 = in[30];
  charger_bus->ChargerAcMaximumChargePower.EVSEMaximumChargePower_L3 = in[31];

  charger_bus->ChargerAcMinimumChargePower.EVSEMinimumChargePower = in[32];
  charger_bus->ChargerAcMinimumChargePower.EVSEMinimumChargePower_L2 = in[33];
  charger_bus->ChargerAcMinimumChargePower.EVSEMinimumChargePower_L3 = in[34];

  charger_bus->ChargerAcChargingSafety.MaximumPowerAsymmetry = in[35];
  charger_bus->ChargerAcChargingSafety.EVSEPowerRampLimitation = in[36];

  charger_bus->ChargerChargingService.GenerateMode = in[39];
  charger_bus->ChargerChargingService.BPTChannel = in[40];
  charger_bus->ChargerChargingService.GridCodeIslandingDetectionMethod = in[41];

  charger_bus->ChargerAcMaximumDischargePower.EVSEMaximumDischargePower = in[42];
  charger_bus->ChargerAcMaximumDischargePower.EVSEMaximumDischargePower_L2 = in[43];
  charger_bus->ChargerAcMaximumDischargePower.EVSEMaximumDischargePower_L3 = in[44];

  charger_bus->ChargerAcMinimumDischargePower.EVSEMinimumDischargePower = in[45];
  charger_bus->ChargerAcMinimumDischargePower.EVSEMinimumDischargePower_L2 = in[46];
  charger_bus->ChargerAcMinimumDischargePower.EVSEMinimumDischargePower_L3 = in[47];

  charger_bus->ChargerAcFrtLvrt.LVRTMomentaryCessationEnable = in[48];
  charger_bus->ChargerAcFrtLvrt.LVRTMomentaryCessationVoltage = in[49];
  charger_bus->ChargerAcFrtLvrt.LVRTMomentaryCessationRecoveryV = in[50];

  charger_bus->ChargerAcFrtHvrt.HVRTMomentaryCessationEnable = in[51];
  charger_bus->ChargerAcFrtHvrt.HVRTMomentaryCessationVoltage = in[52];
  charger_bus->ChargerAcFrtHvrt.HVRTMomentaryCessationRecoveryV = in[53];

  charger_bus->ChargerAcFrtReactivePower.ReactivePowerResponseTimeEnable = in[54];
  charger_bus->ChargerAcFrtReactivePower.ReactivePowerControlResponse = in[55];
}

