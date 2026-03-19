#include <efi.h>
#include <efilib.h>

#define BIT_BOOL(value) value ? L"SET" : L"UNSET"
#define IA32_TME_ACTIVATE 0x982

STATIC UINT64 rdmsr(UINT32 msr) {
#ifdef _DEBUG
  return 0x000400040000002B;
#endif

  UINT32 low, high;
  __asm__ __volatile__ ("rdmsr" : "=a"(low), "=d"(high) : "c"(msr));

  return ((UINT64) high << 32) | low;
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
  InitializeLib(ImageHandle, SystemTable);

  UINT64 msr_value = rdmsr(IA32_TME_ACTIVATE);

  UINT8 lock_ro          =  msr_value        & 0x1;
  UINT8 hw_enc_enable    = (msr_value >>  1) & 0x1;
  UINT8 save_key_standby = (msr_value >>  3) & 0x1;
  UINT8 tme_policy       = (msr_value >>  4) & 0xF;
  UINT8 tme_bypass       = (msr_value >> 31) & 0x1;

  Print(L"Lock RO:                          %s\n", BIT_BOOL(lock_ro));
  Print(L"Hardware Encryption Enable:       %s\n", BIT_BOOL(hw_enc_enable && !tme_bypass));
  Print(L"Save TME key for Standby:         %s\n", BIT_BOOL(save_key_standby));

  UINT16 *tme_policy_text = L"Unknown";

  switch (tme_policy) {
    case 0:
      tme_policy_text = L"AES-XTS-128";
      break;
    case 2:
      tme_policy_text = L"AES-XTS-256";
      break;
  }

  Print(L"TME Policy/Encryption Algorithm:  %s\n", tme_policy_text);

#ifndef _DEBUG
  while(1);
#endif

  SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);

  return EFI_SUCCESS;
}
