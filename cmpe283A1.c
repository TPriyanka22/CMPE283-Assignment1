/*  
 CMPE283A1.c - Kernel module submitted by Priyanka Turumalla - 016672669 ( As a part of Assignment1)
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <asm/msr.h>

#define MAX_MSG 80

/*
 * Model specific registers (MSRs) by the module.
*/
#define IA32_VMX_PINBASED_CTLS	0x481
#define IA32_VMX_PROCBASED_CTLS 0x482
#define IA32_VMX_PROCBASED_CTLS2 0x48B
#define IA32_VMX_EXIT_CTLS 0x483
#define IA32_VMX_ENTRY_CTLS 0x484
#define IA32_VMX_PROCBASED_CTLS3 0x492

/*
 * struct caapability_info
 *
 * Represents a single capability (bit number and description).
 * Used by report_capability to output VMX capabilities.
 */
struct capability_info {
	uint8_t bit;
	const char *name;
};


/*
 * Pinbased capabilities
 SDM volume 3, section 24.6.1
 */
struct capability_info pinbased[5] =
{
	{ 0, "External Interrupt Exiting" },
	{ 3, "NMI Exiting" },
	{ 5, "Virtual NMIs" },
	{ 6, "Activate VMX Preemption Timer" },
	{ 7, "Process Posted Interrupts" }
};

/*
 * Primary Processor-based capabilities
    SDM volume 3, section 24.6.2
 */
struct capability_info primaryprocbased[21] =
{
    {2, "Interrupt window exiting" },
    {3, "Use TSC offsetting" },
    {7, "HLT Exiting" },
    {9, "INVLPG Exiting" },
    {10, "MWAIT Exiting" },
    {11, "RDPMC Exiting" },
    {12, "RDTSC Exiting" },
    {15, "CR3-load Exiting" },
    {16, "CR3-store Exiting" },
    {17, "Activate tertiary controls" },
    {19, "CR8-load Exiting" },
    {20, "CR8-store Exiting" },
    {21, "Use TPR shadow" },
    {22, "NMI-window Exiting" },
    {23, "MOV-DR Exiting" },
    {24, "Unconditional I/O Exiting" },
    {25, "Use I/O bitmaps" },
    {27, "Monitor trap flag" },
    {29, "MONITOR Exiting" },
    {30, "PAUSE Exiting" },
    {31, "Activate secondary controls" }

};

/*
 * Secondary Processor-based capabilities
    SDM volume 3, section 24.6.2, Table 224.7
    */
struct capability_info secondaryprocbased[28] =
{
    {0, "Virtualize APIC accesses" },
    {1, "Enable EPT" },
    {2, "Descriptor-table Exiting" },
    {3, "Enable RDTSCP" },
    {4, "Virtualize x2APIC mode" },
    {5, "Enable VPID" },
    {6, "WBINVD Exiting" },
    {7, "Unrestricted guest" },
    {8, "APIC-register virtualization" },
    {9, "Virtual-interrupt delivery" },
    {10, "PAUSE-loop Exiting" },
    {11, "RDRAND Exiting" },
    {12, "Enable INVPCID" },
    {13, "Enable VM functions" },
    {14, "VMCS shadowing" },
    {15, "Enable ENCLS Exiting" },
    {16, "RDSEED Exiting" },
    {17, "Enable PML" },
    {18, "EPT-violation #VE" },
    {19, "Conceal VMX from PT" },
    {20, "Enable XSAVES/XRSTORS" },
    {22, "Mode-based execute control for EPT" },
    {23, "Sub-page write permissions for EPT" },
    {24, "Intel PT uses guest physical addresses" },
    {25, "Use TSC scaling" },
    {26, "Enable user wait and pause" },
    {27, "Enable PCONFIG"}
    {28, "Enable ENCLV Exiting" }
};


/*
 * Tertiary Processor-based capabilities
   SDM volume 3, section 24.6.2 , Table 24.8
*/
struct capability_info tertiaryprocbased[4] =
{     
    {0, "LOADIWKEY Exiting" },
    {1, "Enable HLAT" },
    {2, "EPT paging-write control" },
    {3, "Guest-paging verification" }
};

/*
 * VM-Exit Controls capabilities
    SDM volume 3, section 24.7.1, Table 24.13
 */
struct capability_info vmexitcontrols[17] =
{	
    {2, "Save debug controls" },
    {9, "Host address-space size" },
    {12, "Load IA32_PERF_GLOBAL_CTRL" },
    {15, "Acknowledge interrupt on exit" },
    {18, "Save IA32_PAT" },
    {19, "Load IA32_PAT" },
    {20, "Save IA32_EFER" },
    {21, "Load IA32_EFER" },
    {22, "Save VMXpreemption timer value" },
    {23, "Clear IA32_BNDCFGS" },
    {24, "Conceal VMX from PT" },
    {25, "Clear IA32_RTIT_CTL" },
    {26, "Clear IA32_LBR_CTL" },
    {28, "Load CET state" },
    {29, "Load PKRS" },
    {30, "Save IA32_PERF_GLOBAL_CTL"}
    {31, "Activate secondary controls" }
};


/*
 *  VM-Entry Controls capabilities
    SDM volume 3, section 24.8.1 Table 24.15
 */
struct capability_info vmentrycontrols[13] =
{     
    {2, "Load debug controls" },
    {9, "IA-32e mode guest" },
    {10, "Entry to SMM" },
    {11, "Deactivate dualmonitor treatment" },
    {13, "Load IA32_PERF_GLOBAL_CTRL" },
    {14, "Load IA32_PAT" },
    {15, "Load IA32_EFER" },
    {16, "Load IA32_BNDCFGS" },
    {17, "Conceal VMX from PT" },
    {18, "Load IA32_RTIT_CTL" },
    {20, "Load CET state" },
    {21, "Load guest IA32_LBR_CTL" },
    {22, "Load PKRS" }
};
/*
 * report_capability
 *
 * Reports capabilities present in 'cap' using the corresponding MSR values
 * provided in 'lo' and 'hi'.
 *
 * Parameters:
 *  cap: capability_info structure for this feature
 *  len: number of entries in 'cap'
 *  lo: low 32 bits of capability MSR value describing this feature
 *  hi: high 32 bits of capability MSR value describing this feature
 */
void
report_capability(struct capability_info *cap, uint8_t len, uint32_t lo,
    uint32_t hi)
{
	uint8_t i;
	struct capability_info *c;
	char msg[MAX_MSG];

	memset(msg, 0, sizeof(msg));

	for (i = 0; i < len; i++) {
		c = &cap[i];
		snprintf(msg, 79, "  %s: Can set=%s, Can clear=%s\n",
		    c->name,
		    (hi & (1 << c->bit)) ? "Yes" : "No",
		    !(lo & (1 << c->bit)) ? "Yes" : "No");
		printk(msg);
	}
}

/*
 * detect_vmx_features
 *
 * Detects and prints VMX capabilities of this host's CPU.
 */
void
detect_vmx_features(void)
{
	uint32_t lo, hi,check_secondary_exists, check_teritary_exists ;

	/* Pinbased controls */
	rdmsr(IA32_VMX_PINBASED_CTLS, lo, hi);
	pr_info("Pinbased Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(pinbased, 5, lo, hi);

    
    /* Primary Processor-Based VM-Execution controls */
	rdmsr(IA32_VMX_PROCBASED_CTLS, lo, hi);
	pr_info(" Primary Processor-Based Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(primaryprocbased, 21, lo, hi);

    
    /* Secondary Processor-Based VM-Execution Controls */
   
    rdmsr( IA32_VMX_PROCBASED_CTLS2, lo, hi);
    pr_info("Secondary Processor-Based Controls MSR: 0x%llx\n",
            (uint64_t)(lo | (uint64_t)hi << 32));
    report_capability(secondaryprocbased, 28, lo, hi);
   
    
    /* Tertiary Procbased controls */

	rdmsr(IA32_VMX_PROCBASED_CTLS3, lo, hi);
	pr_info("Tertiary Processor-Based Controls MSR: 0x%llx\n",
		    (uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(tertiaryprocbased, 4, lo, hi);	
   

    /* Exit controls */
	rdmsr(IA32_VMX_EXIT_CTLS, lo, hi);
	pr_info("Exit Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(vmexitcontrols, 17, lo, hi);
	
	
	/* Entry controls */
	rdmsr(IA32_VMX_ENTRY_CTLS, lo, hi);
	pr_info("Entry Controls MSR: 0x%llx\n",
		(uint64_t)(lo | (uint64_t)hi << 32));
	report_capability(vmentrycontrols, 13, lo, hi);

}

/*
 * init_module
 *
 * Module entry point
 *
 * Return Values:
 *  Always 0
 */
int
init_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Start\n");
	detect_vmx_features();
	/* 
	 * A non 0 return means init_module failed; module can't be loaded. 
	 */
	return 0;
}

/*
 * cleanup_module
 *
 * Function called on module unload
 */
void
cleanup_module(void)
{
	printk(KERN_INFO "CMPE 283 Assignment 1 Module Exits\n");
}

MODULE_LICENSE("GPL");