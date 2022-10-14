#include "elf64.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/types.h>
#include <unistd.h>
#include <syscall.h>
pid_t run_target(const char* progname, char** argv)
{
	pid_t pid;
	pid = fork();
	if (pid > 0)
	{
		return pid;	
	}
	else if (pid == 0)
	{
		if(ptrace(PTRACE_TRACEME,0,NULL,NULL)<0)
		{
			perror("ptrace");
			exit(1);
		}
		execv(progname,argv+2);
	}
	else
	{
		perror("fork");
		exit(1);
	}
}

void run_debugger(pid_t child_pid, unsigned long address, bool is_dynamic)
{
	int wait_status;
	int counter = 1;
	struct user_regs_struct regs;
	unsigned long function_rsp;
	wait(&wait_status);
	bool first_call_is_over = false;
	unsigned long old_address = address;

	
	
	//if the simbol is dynamic, get the address of the first instruction in the plt
	if (is_dynamic)
	{
		address = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)address,NULL);
		address -= 6;
		
	}
	
	// making the trap
	long data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)address,NULL);
	unsigned long data_trap = (data & 0xFFFFFFFFFFFFFF00) | 0xCC;
	ptrace(PTRACE_POKETEXT, child_pid, (void*)address, (void*)data_trap);
	
	
	ptrace(PTRACE_CONT, child_pid, NULL, NULL);
	wait(&wait_status);
	
	unsigned long ret_rip;
	while(WIFSTOPPED(wait_status))
	{		
		ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
		ptrace(PTRACE_POKETEXT, child_pid, (void*)address, (void*)data);
		regs.rip -= 1;
		ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
		
		ret_rip = ptrace(PTRACE_PEEKTEXT, child_pid, regs.rsp,NULL);
			
		long ret_data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)ret_rip,NULL);
		unsigned long ret_data_trap = (ret_data & 0xFFFFFFFFFFFFFF00) | 0xCC;
		ptrace(PTRACE_POKETEXT, child_pid, (void*)ret_rip, (void*)ret_data_trap);
		
		ptrace(PTRACE_CONT, child_pid,0,0);
		wait(&wait_status);
		
		if (!first_call_is_over && is_dynamic) 
		{
			first_call_is_over = true;			
			address = old_address;
			address = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)address,NULL);
			data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)address,NULL);
			data_trap = (data & 0xFFFFFFFFFFFFFF00) | 0xCC;
		}
		
		ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
		ptrace(PTRACE_POKETEXT, child_pid, (void*)ret_rip, (void*)ret_data);
		regs.rip -= 1;
		ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
		
		printf("PRF:: run #%d returned with %d\n",counter,(int)regs.rax);
		counter++;

		//return the trap for next calls
		
		ptrace(PTRACE_POKETEXT, child_pid, (void*)address, (void*)data_trap);
		ptrace(PTRACE_CONT, child_pid,0,0);
		wait(&wait_status);
	}
	
}

int main (int argc, char** argv)
{
		
	Elf64_Ehdr elf_file_header;	
	FILE* fp;
	fp = fopen(argv[2],"r");
	
	fread (&elf_file_header, sizeof(elf_file_header), 1 , fp);
	if (elf_file_header.e_type != 2)
	{
		printf("PRF:: %s not an executable! :(\n" , argv[2]);
		fclose(fp);
		return 0;
	}
	
	int num_of_sections = elf_file_header.e_shnum;
	


	/* Getting the shstrtab */
	int shstrtab_index = elf_file_header.e_shstrndx;
	Elf64_Shdr shstrtab;
	fseek(fp, elf_file_header.e_shoff + sizeof(shstrtab)*shstrtab_index, SEEK_SET);
	fread (&shstrtab, sizeof(shstrtab), 1 , fp);
	int shstrtab_offset = shstrtab.sh_offset;
	
	/* prepare the strtab and symtab */
	Elf64_Shdr strtab_sec;
	Elf64_Shdr symtab_sec;	
	Elf64_Shdr rela_plt;
	Elf64_Shdr dynsym;
	Elf64_Shdr dynstr;
	bool is_there_rela = false;



	/* Getting all the sections name */
	char* name = malloc (shstrtab.sh_size + 1);
	fseek(fp, shstrtab_offset, SEEK_SET);
	fread (name, shstrtab.sh_size+1, 1 , fp);
	
	/* Getting all the sections header table */	
	Elf64_Shdr* sections_table = malloc (num_of_sections*sizeof(Elf64_Shdr));
	fseek(fp, elf_file_header.e_shoff, SEEK_SET);
	fread (sections_table, num_of_sections*sizeof(Elf64_Shdr),1,fp);	
	
	/* Searching for the relevant simbol */
	for (int i=0 ; i< num_of_sections; i++)
	{
		/* Getting the section name */
		char* sec_name = &name[sections_table[i].sh_name];

		if (!strcmp(sec_name, ".symtab\0"))
		{
			symtab_sec = sections_table[i];
			
		}
		if (!strcmp(sec_name, ".strtab\0"))
		{
			strtab_sec = sections_table[i];
		}
		if (!strcmp(sec_name, ".rela.plt\0"))
		{
			rela_plt = sections_table[i];
			is_there_rela = true;
			
		}
		if (!strcmp(sec_name, ".dynsym\0"))
		{
			dynsym = sections_table[i];
			
		}
		if (!strcmp(sec_name, ".dynstr\0"))
		{
			dynstr = sections_table[i];
			
		}
		
		
	}
	free(name);
	free(sections_table);
	
	
	
	Elf64_Sym symbol;
	char* sym_name = malloc (strtab_sec.sh_size + 1);
	bool func_exist = false;
	fseek(fp, strtab_sec.sh_offset , SEEK_SET);
	fread (sym_name, strtab_sec.sh_size+1, 1 , fp);

	Elf64_Sym* symtab_table = malloc (symtab_sec.sh_size);
	fseek(fp, symtab_sec.sh_offset , SEEK_SET);
	fread (symtab_table, symtab_sec.sh_size, 1 , fp);
		
	for (int i=0 ; i< symtab_sec.sh_size/sizeof(symbol); i++)
	{
		
		
		/* Getting the symbol name */
		char* sec_name = &sym_name[symtab_table[i].st_name];
		if (!strcmp(sec_name, argv[1]))
		{
			func_exist = true;
			symbol = symtab_table[i];
			if (ELF64_ST_BIND(symbol.st_info) == 1)
			{
				break;
			}
			
		}
		
		
	}
	free(symtab_table);
	free(sym_name);

	if (!func_exist)
	{
		printf("PRF:: %s not found!\n", argv[1]);
		fclose(fp);
		return 0;
	}


	if (ELF64_ST_BIND(symbol.st_info) != 1)
	{		
		printf("PRF:: %s is not a global symbol! :(\n", argv[1]);
		fclose(fp);
		return 0;
	}
	
	long long func_offset = 0;
	if (symbol.st_shndx == SHN_UNDEF)
	{		
		
		char* dyn_str = malloc (dynstr.sh_size + 1);
		fseek(fp, dynstr.sh_offset , SEEK_SET);
		fread (dyn_str, dynstr.sh_size+1, 1 , fp);
		
		Elf64_Rela* rela_table = malloc (rela_plt.sh_size);
		fseek (fp, rela_plt.sh_offset , SEEK_SET);
		fread (rela_table, rela_plt.sh_size, 1 , fp);
		
		Elf64_Sym* dynsym_table = malloc (dynsym.sh_size);
		fseek (fp, dynsym.sh_offset , SEEK_SET);
		fread (dynsym_table, dynsym.sh_size, 1 , fp);

		/*check if dynamic symbol exists */
		for (int i=0 ; i< rela_plt.sh_size/sizeof(Elf64_Rela); i++)
		{
			
			unsigned long offset = 	ELF64_R_SYM(rela_table[i].r_info);
			
			char* sec_name = &dyn_str[dynsym_table[offset].st_name];
			
			if (!strcmp(sec_name, argv[1]))
			{
				func_offset = rela_table[i].r_offset;
				break;
				
			}
			
		}
		
		free(dyn_str);
		free(rela_table);
		free(dynsym_table);

	}
	
	
	
	
	pid_t child_pid;
	child_pid = run_target(argv[2], argv);
	unsigned long func_position = 0;
	
	if (symbol.st_shndx == SHN_UNDEF)
	{		
		func_position = func_offset;
		run_debugger(child_pid,func_position, true);
	}
	else
	{
		func_position = symbol.st_value;
		run_debugger(child_pid,func_position, false);
	}
	
	/* DEBUGGING */
		//printf ("%s in address: %lx\n",argv[1],func_position);
		
	/* *******/		
	
	
	fclose(fp);
	return 0;
}