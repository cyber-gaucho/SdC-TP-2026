#include <efi.h>
#include <efilib.h>

// DECLARACIÓN GLOBAL Y VOLÁTIL
volatile unsigned char mi_breakpoint = 0xCC;

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
    InitializeLib(ImageHandle, SystemTable);
    
    SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Iniciando analisis de seguridad...\r\n");

    if (mi_breakpoint == 0xCC) {
        SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Breakpoint estatico alcanzado.\r\n");
    }

    return EFI_SUCCESS;
}