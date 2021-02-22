#Organizacja i rozwój projektów Open Source



#HoroOS


Mateusz Gruchlik
Przemysław Pigoń


Ogólne informacje

    HoroOS jest to prekursor do 32 bitowego systemu operacyjnego open source opierającego się o architekturę jądra UNIX, nazwa pochodzi od imienia głównej bohaterki serii Spice&Wolf. System powstał w celach edukacyjnych.
Zawartość projektu

Pliki nagłówkowe oraz źródłowe:
HoroKernel.c
    Główny plik projektu będący swoistym “main” całego systemu operacyjnego, plik zawiera wszystkie metody przygotowujące sprzęt do działania.  
HoroBoot.s
    Plik asemblerowy wymuszający przejście z BIOS’a do systemu właściwego systemu operacyjnego

.set MAGIC,     0x1badb002
.set FLAGS,     (1<<0 | 1<<1)
.set CHECKSUM,    -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text

.extern HoroKernel

.global loader


loader:
    cli
    mov $kernel_stack, %esp
    push %eax
    push %ebx
    call HoroKernel

_stop:                
    cli
    hlt
    jmp _stop
    
.section .bss
.space 64*1024*1024    ; # 2MiB
kernel_stack:
HoroLinker.ld
Plik linker linkujący oraz informujący o segmentach w pamięci zawierających kod wykonywalny oraz dane, konieczny do kompilacji i uruchamiania metod w języku C.  
kernelstdio.h/.c
    Pliki zawierające implementację obsługi terminala oraz metody pozwalające użytkownikowi na interakcję z komputerem. Domyślnie pamięć znaków terminala w trybie tekstowym karty graficznej jest pod adresem 0xB8000 i po odpowiednim zamaskowaniu znaku możliwe jest jego wyświetlenie na ekranie.

Metody:

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
uint16_t vga_entry(unsigned char uc, uint8_t color)
Metody te przygotowują symbol do wyświetlania na ekranie maskując kolor tła, symbol oraz kolor symbolu.

size_t strlen(const char* str);
    Zwraca długość ciągu znaków.

void strrev(char* str, uint32_t length);
    Odwraca ciąg znaków.

void terminal_initialize(void);
    Inicjalizuje terminal.

void terminal_setcolor(uint8_t color);
Ustawia kolor czcionki terminala.
  
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y); 
Ustawienie karetki terminala na konkretną pozycję.
 
void terminal_clear();
    Wyczyszczenie ekranu.

void terminal_scroll_up();
    Przesunięcie “ściany tekstu” o wiersz w górę,

void terminal_putchar(char c);
Wyświetla znak w terminalu. 
 

void terminal_write(const char* data, size_t size);
    Wyświetla ciąg znaków w terminalu.

void printf(const char* data);
    Ulepszona wersja terminal_write.

char* itoa(int32_t num, char* str, uint8_t base); 
    Przerabia int na ascii char.


 
GDT.c/.h
    Implementacja tablicy deskryptorów (tworzenie, uruchamianie oraz obsługa), w trybie chronionym procesora x86 struktura ulokowana w dedykowanym segmencie, zawierająca deskryptory wszystkich segmentów znajdujących się w pamięci operacyjnej.

Plik GDT.h zawiera definicję pojedynczego deskryptora:

struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
    
}__attribute__((packed));

oraz specjalnego wskaźnika na tablicę deskryptorów:

struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
    
}__attribute__((packed));

sposób implementacji obu tych struktur jest ściśle określony przez standard x86.

Metody:

void gdt_flush()
Metoda zdefiniowana w GDTasm.s umożliwiająca usunięcie starej tablicy deskryptorów i przygotowująca procesor przed wgraniem nowej oraz ustawienie nowej w procesorze jako aktualną.

gdt_set_gate(uint32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
Metoda ta pozwala na ustawienie wpisu w tablicy deskryptorów.

gdt_install()
Metoda ta wykonuje wszystkie potrzebne operacje związane z ustawieniem tablicy deskryptorów procesora oraz na koniec wyświetla informację o statusie wykonanej operacji.

GDTasm.s
    Plik napisany w języku assembler ustawiający poszczególne rejestry procesora odpowiedzialne za tablicę deskryptorów, jest on wykorzystywany przez GDT.c. 

Kod źródłowy: 

.section .text

.global gdt_flush
.extern gp

gdt_flush:

    lgdt gp
    jmp $0x08, $flush2
    
flush2:
    mov $0x10, %eax
    mov %eax,  %ds
    mov %eax,  %es
    mov %eax,  %fs
    mov %eax,  %gs
    mov %eax,  %ss
    ret


interrupt.h/.c
    Pliki implementujące obsługę przerwań. Obsługa przerwań jest konieczna by sprzęt mógł się komunikować z użytkownikiem. O przerwania opiera się obsługa myszki czy klawiatury. Procesory są w stanie obsłużyć do 256 przerwań. 

Plik interrupt.h zawiera definicję opisu przerwania:

struct idt_entry
{
    uint16_t handlerAddressLow;
    uint16_t gdt_codeSegmentSelector;
    uint8_t reserved;
    uint8_t access;
    uint16_t handlerAddressHigh;
    
}__attribute__((packed));

oraz wskaźniku na całą tablicę przerwań:

struct idt_ptr
{
    uint16_t limit;
    uint32_t base;
    
}__attribute__((packed));

Metody:

void InitInterupts()
    Zbiorcza metoda przeprowadzająca cały proces aktywacji przerwań.

void ActivateInterrupts()
Aktywacja przerwań poprzez wysłanie do procesora odpowiedniego rozkazu.

void DeactivateInterrupts()
    Jak wyżej tylko przy wyłączeniu przerwań.

void SetIDTentry(uint8_t INT_number, uint16_t CodeSegmentSelectorOffset, void(*handler)(), uint8_t DescriptorPrivilidgeLevel, uint8_t DescriptorType)
    Ustawienie pojedynczego wpisu do tablicy przerwań.




uint32_t HandleInterrupt(uint8_t INT_number, uint32_t ESP)
    Obsługa przerwania o konkretnym numerze dostarczanym przez procesor.

void HandleInterruptRequest0x00()
void HandleInterruptRequest0x01()
void HandleInterruptRequest0x0C()
    Puste definicje metod, potrzebne tylko jako wskaźnik do metod z pliku asm.

void IgnoreInterruptRequest()
    Ignorowanie przerwania.

void HelpMe()
    Metoda która pomogła jednemu z autorów systemu operacyjnego ominąć szaleństwo i wizytę w lokalnym zakładzie psychiatrycznym. Pokazała w którym momencie procesor wchodzi w “triple fault”.

void idt_install()
    Pusta metoda której definicja znajduje się w pliku asm, definicja jest po to by móc się do niej odwołać w pliku .c. Przygotowuje procesor do uruchomienia obsługi przerwań ładując tablice przerwań.
interruptasm.s

    Plik przygotowujący procesor do obsługi przerwania, między innymi zapisuje oraz odzyskuje dane rejestrów ze stosu procesora.

Fragment kodu obsługujący stos

... 
int_bottom:
    
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call HandleInterrupt
    mov %eax, %esp
    
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa
...
port.h/.c
    Pliki implementujące metody obsługujące wszelakie porty służących do komunikacji procesora ze sprzętem.

Zdefiniowane makra wykorzystywane w obsłudze transmisji danych poprzez porty:

#define PORT_MODE_8           0
#define PORT_MODE_16            1
#define PORT_MODE_32            2
#define PORT_MODE_8_SLOW        3
    Rozmiar danych przechodzących przez port.

#define EOI_M     0
#define EOI_MS    1
    Flaga End Of Interupt.
    
#define HEX_PORT_NUMBER_PIC_MCP    0x20
#define HEX_PORT_NUMBER_PIC_MDP    0x21
#define HEX_PORT_NUMBER_PIC_SCP    0xA0
#define HEX_PORT_NUMBER_PIC_SDP    0xA1
Programmable interrupt controller master oraz slave, numery potrzebne procesorowi do komunikacji.

#define HEX_PORT_NUMBER_KBMS_DP    0x60
#define HEX_PORT_NUMBER_KBMS_CP    0x64
    Numer portu do komunikacji klawiatury i myszki.

#define HEX_PORT_NUMBER_PCI_CONFIG_DATA     0xCFC
#define HEX_PORT_NUMBER_PCI_CONFIG_ADDR        0xCF8
    Numery portów do konfiguracji i komunikacji urządzeń PCI.
    
Struktura przechowująca dane portu oraz odbudowująca zwracane wartości:

struct InternalPortVariables
{
        uint8_t     *WritePort8bit;
uint16_t     *WritePort16bit;
    uint32_t     *WritePort32bit;
    
    uint8_t     *ResutWrapper8bit;
    uint16_t     *ResutWrapper16bit;
    uint32_t     *ResutWrapper32bit;
    
    uint8_t v0x20 ;
    uint8_t status;
};

Metody:

void writePort(uint8_t Mode, uint16_t PortNumber, void* data);
void readPort(uint8_t Mode, uint16_t PortNumber, void* result);
Zapisanie wartości do portu i odczytanie wartości z portu. Budowa metody pozwala na przekazywanie i zwracanie różnych typów danych poprzez wykorzystanie pustego wskaźnika jako argumentu metody.

void InitPIC();
    Inicjalizacja PIC’a i przygotowanie go obustronnej komunikacji.

void EOI(uint8_t type);
    End Of Interupt, wysłanie do portu informacji o zakończeniu obsługi przerwania myszki lub klawiatury.

void WaitFor0x60Read();
void WaitFor0x60_0x64Write();
    Niektóre operacja na portach wymagają potwierdzenia zakończenia obsługi zapisu lub odczytu, powyższe metody obsługują port 0x60 oraz 0x64 blokując sekwencję programu czekając aż port skończy przetwarzać dane. 
PS2.h/.c
    Pliki implementujące inicjalizację kontrolera PS2 koniecznego do obsługi myszki i klawiatury (nawet jeśli myszka czy klawiatura jest na USB). Plik zawiera strukturę ze statusem kontrolera oraz inicjalizację.
Mouse.h/.c
    Pliki implementujące obsługę i inicjalizację komunikacji z myszką.

Struktura przechowująca zwrotne dane pochodzące z myszki:

struct MouseState
{

    uint8_t mouse_data[3];
    uint8_t mouse_data_itterator;
    int8_t mouse_X, mouse_Y;
    
    // byte 0 -> | Y overflow | X overflow | Y sign bit | X sign bit | 1 | Middle button | Right button | Left button |
    // byte 1 ->  X movement
    // byte 2 ->  Y movement
        uint8_t status;
};

Metody:

void InitMouse();
    Implementacja inicjalizacji myszki.

void GetDataFromMouse();
    Metoda pobierająca aktualne dane z myszki.
Keyboard.h/.c
    Pliki implementujące obsługę i inicjalizację komunikacji z klawiaturą.

Struktura przechowująca zwrotne dane pochodzące z klawiatury oraz pomocniczą maskę przydatną przy dekodowaniu kodu klawisza:

struct KeyboardState
{
    uint8_t Key;
    uint8_t SCAKeyMask;// | L Shift | R Shift | L Ctrl | R Ctrl | L Alt | R Alt | E0 | 0 |
    
    uint8_t status;
};

void InitKeyboard();
    Implementacja inicjalizacji klawiatury.

void GetDataFromKeyboard();
    Metoda pobierająca aktualne dane z klawiatury.

char GetCharFromKeyCode();
    Metoda tłumacząca kod klawisza i maski na symbol; 

PCI.h/.c
    Pliki implementujące obsługę obsługę magistrali PCI.

Struktura przechowująca komplet informacji o urządzeniu podłączonym do magistrali, najważniejszym jest numer przerwania oraz ID:

struct PCIDescriptorEntity
{
    uint32_t portBase;
    uint32_t interrupt;
    
    uint16_t bus;
    uint16_t device;
    uint16_t function;
    
    uint16_t vendor_id;
    uint16_t device_id;
    
    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;
    
    uint8_t revision;
};

Struktura przechowująca aktualnie obsługiwane urządzenie, oraz gdy to potrzebne także odpowiedź: 

struct PCIData
{
    uint32_t id;
    uint32_t result;
    
    uint8_t DescriptorTableItterator;
};

Metody:

void InitPCI()
    Metoda inicjalizująca urządzenia oraz magistralę PCI, została zaimplementowana w klasycznym stylu starych BIOS’ów wyświetlając komplet informacji na temat podłączonych urządzeń do magistrali.


void createPCIdtEntity(uint16_t bus, uint16_t device, uint16_t function)    
Metoda ta tworzy wpis w tablicy urządzeń PCI.

uint32_t readPCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)

void writePCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t *value)
    Metody te służą do komunikacji z urządzeniem PCI, do implementacji zostały wykorzystane metody write port oraz read port.

bool isDeviceHasFunctions(uint16_t bus, uint16_t device);
    Metoda ta służy do wykrywania funkcji urządzenia oraz sprawdzania czy urządzenie istnieje. Domyślnie urządzenie PCI musi mieć funkcję, brak funkcji świadczy o tym że urządzenie jest zepsute lub nie istnieje. 


#Proces developerski

    Programowanie i kompilacja systemu operacyjnego pod GCC wymaga użycia specjalnych flag takich jak -ffreestanding oraz -nostdlib, pierwsza informuje o tym że program wyjściowy jest samodzielnie działającym systemem a druga by GCC wiedział że nie ma szukać biblioteki standardowej c. Brak stdlib oznacza że trzeba implementować wszystkie metody takie jak printf, malloc czy random od podstaw. 

    Jako bootloader został wykorzystany GRUB2 gdyż znacznie upraszcza proces przejścia z BIOS do OS.



    W aktualnej fazie projektu HoroOS jest testowany wyłącznie wewnątrz maszyny wirtualnej QEMU pod systemem linux. QEMU umożliwia bardzo dokładną analizę kodu wykonywalnego, jednym z sposobów jest analiza wygenerowanego kodu w języku assembler, dzięki temu można analizować prawie na najniższym poziomie co się dokładnie dzieje wewnątrz systemu podczas działania.




    W przypadku dalszego rozwoju systemu planowana jest próba tak zwanego “nestingu”, gdy system osiągnie kontrolę nad wszystkimi urządzeniami I/O oraz będą zaimplementowane metody umożliwiające programowanie, można rozwijać system operacyjny wewnątrz niego samego, ten proces zwany jest nestingiem. 

#Aktualne funkcjonalności

    System aktualnie jest ustawiony w tryb 32bit, czyli obsługuje do 4GB pamięci RAM, działa w oparciu o statyczną pamięć, jest w trybie tekstowym, posiada obsługę klawiatury i myszki, w przyszłości jest planowane dodanie trybu graficznego.



   
