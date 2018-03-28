
#include "_signal.h"
#include "help.h"

pthread_mutex_t  iic_lock;
pthread_mutex_t  check_lock;

extern sig_atomic_t      sig_exit;
extern rom_device        rdev;
extern pthread_mutex_t   channel_lock;
extern adev_prop_t      *adevp;

void init_net();

int main()
{
	sigset_t    set;

	if (init_signals()) {
		error_log("init_signals() fail");
		exit(0);
	}

	sigfillset(&set);

	sigdelset(&set, SIGILL);
	sigdelset(&set, SIGFPE);
	sigdelset(&set, SIGSEGV);
	sigdelset(&set, SIGBUS);
	sigdelset(&set, SIGCHLD);

	if (pthread_sigmask(SIG_BLOCK, &set, NULL) == -1) {
		error_log("sigprocmask() fail");
		exit(0);
	}

	show_version_info();
	
	pthread_mutex_init(&iic_lock, 0);
	pthread_mutex_init(&check_lock, 0);

	util::color_led(LED_GREEN);

	adevp->m_genre = check_device();

#if (RLOG)
	if (util::set_log_server()) {
		error_log("set_log_server() fail");
	}
#endif

	init_net();

	__watch watch;
	watch.init();

#if (RUN_MAP)
	run_chart::mc_mnt();
#endif

	oled oled_;

#if (USE_4735) || (USE_476X)

	af_channel af_channels[] = {
		af_channel(0, &watch),
		af_channel(1, &watch),
		af_channel(2, &watch),
		af_channel(3, &watch)
	};

	init_adev(af_channels,&watch,&oled_);
	
	adevp->m_net_cmd = new net_cmd();
	adevp->m_net_cmd->init_net_cmd();

	oled_.init_oled();

#else

	__switch switch_;

	/* if dynamically...fail you...*/
	al_channel al_channels[] = {
		al_channel(0, &watch),
		al_channel(1, &watch),
		al_channel(2, &watch)
	};
	
	ip_channel ip_channels[] = {
		ip_channel(adevp->m_al_num + 0),
		ip_channel(adevp->m_al_num + 1)
	};
	
	adev_out aout[] = {
		adev_out(0)	
	};

	init_adev(al_channels,ip_channels,
		      aout,&watch,&oled_,&switch_);
	
	adevp->m_net_cmd = new net_cmd();
	adevp->m_net_cmd->init_net_cmd();

	oled_.init_oled();
	switch_.init_switch();

#endif

	sigemptyset(&set);

	for ( ;; ) {

		sigsuspend(&set);

		printf("user interrupt\n");

		if (sig_exit) {
			sig_exit = 0;
			break;
		}
	}
	
#if (RUN_MAP)
		run_chart::mc_umnt();
#endif
        
#if (USE_4735) || (USE_476X)	
    pthread_mutex_destroy(&channel_lock);
#else
    pthread_mutex_destroy(&iic_lock);
    pthread_mutex_destroy(&check_lock);
#endif
    
    delete adevp->m_net_cmd;

	exit(0);
}

void init_net()
{
	struct in_addr     addr;
	__int8             local_ip[16],
	                   net_mask[16],
	                   broadcast[16],
	                   route[16];
	bool               valid = true;
	unsigned __int32   ubroadcast = 0;

#define __NET_CONF   util::m_net
#define __VALF0      util::is_addr_valid
#define __INT_IP     util::__int_ip
#define __STR_IP     util::__str_ip

	rdev.read_net_config(__NET_CONF);

	if (!__VALF0(__STR_IP(__NET_CONF.ipaddr))) {
		__NET_CONF.ipaddr = __INT_IP("192.168.110.4");
		valid = false;
		//debug_log("ipaddr = %#x set default", net_conf.ipaddr);
	}

	if (!__VALF0(__STR_IP(__NET_CONF.netmask))) {
		__NET_CONF.netmask = __INT_IP("255.255.255.0");
		valid = false;
		//debug_log("netmask = %#x set default", net_conf.netmask);
	}

	if (!__VALF0(__STR_IP(__NET_CONF.gateway))) {
		__NET_CONF.gateway = __INT_IP("192.168.110.1");
		valid = false;
		//debug_log("gateway = %#x set default", net_conf.gateway);
	}

	if (!valid) {
		rdev.write_net_config(__NET_CONF);
	}

	__NET_CONF.ttl = TTL_DEFAULT;

	addr.s_addr = __NET_CONF.ipaddr;
	sprintf(local_ip, "%s", inet_ntoa(addr));
	addr.s_addr = __NET_CONF.netmask;
	sprintf(net_mask, "%s", inet_ntoa(addr));

	ubroadcast = __NET_CONF.ipaddr | (~__NET_CONF.netmask);
	addr.s_addr = ubroadcast;
	sprintf(broadcast, "%s", inet_ntoa(addr));
	addr.s_addr = __NET_CONF.gateway;
	sprintf(route, "%s", inet_ntoa(addr));

	util::__ifconfig(NET_DEV, local_ip, net_mask, broadcast, route);
}

