/*
 * ** dnsmap - DNS Network Mapper by pagvac
 * ** Copyright (C) 2010 gnucitizen.org
 * **
 * ** This program is free software; you can redistribute it and/or modify
 * ** it under the terms of the GNU General Public License as published by
 * ** the Free Software Foundation; either version 2 of the License, or
 * ** (at your option) any later version.
 * **
 * ** This program is distributed in the hope that it will be useful,
 * ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 * ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * ** GNU General Public License for more details.
 * **
 * ** You should have received a copy of the GNU General Public License
 * ** along with this program; if not, write to the Free Software
 * ** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * */

#define MAXSUBSIZE 	100
#define MAXSTRSIZE 	200
#define BANNER 		"dnsmap 0.25 - DNS Network Mapper by pagvac (gnucitizen.org)\n\n"
#define USAGE 		"usage: dnsmap <target-domain> [options]\noptions:\n"\
			"-w <wordlist-file>\n-r <regular-results-file>\n-c <csv-results-file>\n"\
			"-d <delay-milliseconds>\n"
#define EXAMPLES 	"e.g.:\ndnsmap target-domain.foo\n"\
			"dnsmap target-domain.foo -w yourwordlist.txt -r /tmp/domainbf_results.txt\n"\
			"dnsmap target-fomain.foo -r /tmp/ -d 3000\n"\
			"dnsmap target-fomain.foo -r ./domainbf_results.txt\n\n"
#define INTIPWARN	"[+] warning: internal IP address disclosed\n"
#define SAMESITEXSSWARN "[+] warning: target domain might be vulnerable to \"same site\" scripting (http://snipurl.com/etbcv)\n"
#define WILDCARDWARN	"[+] warning: the target domain might use wildcards. "\
			"dnsmap will try to filter out false positives\n"
#define INPUTERR	"[+] error: entered parameter(s) is/are too long!\n"
#define DELAYINPUTERR	"[+] error: delay must be at least 1 second!\n"
#define DOMAINERR	"[+] error: entered domain is not valid!\n"
#define CREATEFILEERR	"%s\"%s\"!\n\n", "[+] error creating results file on ", argv[(i+1)]
#define OPENFILEERR	"%s\"%s\"!\n\n", "[+] error opening wordlist file ", wordlistFilename
#define OPENDNSMSG	"[+] openDNS detected. good! this might help with performance\n"
#define BUILTINMSG	"%s%s%s\n", "[+] searching (sub)domains for ", argv[1], " using built-in wordlist"
#define EXTERNALMSG	"%s%s%s%s\n", "[+] searching (sub)domains for ", argv[1], " using ", wordlistFilename
#define DELAYMSG	"%s%d%s\n", "[+] using maximum random delay of ", milliseconds, " millisecond(s) between requests"
#define RESULTSMSG1	"[+] %d internal IP addresses disclosed\n", intIPcount
#define RESULTSMSG2	"[+] regular-format results can be found on %s\n", txtResultsFilename
#define RESULTSMSG3	"[+] completion time: %lu second(s)\n", end-start
#define RESULTSMSG4	"[+] %d%s%d%s\n", found, " (sub)domains and ",ipCount, " IP address(es) found"
#define RESULTSMSG5	"[+] csv-format results can be found on %s\n", csvResultsFilename
#define FALSE 0
#define TRUE 1
#define DEBUG 1

// buil-in list of subdomains
// mainly targeting English and Spanish keywords
char sub[][MAXSUBSIZE]=
{
"access",
"accounting",
"accounts",
"admin",
"administrator",
"aix",
"ap",
"apollo",
"archivos",
"aula",
"aulas",
"ayuda",
"backup",
"backups",
"bart",
"bd",
"beta",
"biblioteca",
"billing",
"blackboard",
"blog",
"blogs",
"bsd",
"carro",
"cart",
"cas",
"catalog",
"catalogo",
"catalogue",
"chat",
"chimera",
"chronos", // time server?
"citrix",
"classroom",
"clientes",
"clients",
"connect",
"controller",
"correoweb",
"cpanel",
"csg",
"customers",
"data",
"db",
"dbs",
"dc",
"demo",
"demon",
"demostration",
"descargas",
"developers",
"development",
"diana",
"directory",
"dmz",
"domain",
"domain-controller",
"domaincontroller",
"download",
"downloads",
"ds",
"eaccess",
"ejemplo",
"ejemplos",
"email",
"enrutador",
"eventos",
"events",
"example",
"examples",
"exchange",
"extranet",
"files",
"finance",
"firewall",
"foro",
"foros",
"forum",
"forums",
"freebsd",
"ftp",
"ftpd",
"fw",
"galeria",
"gallery",
"gateway",
"gilford",
"groups",
"groupwise",
"guest",
"guia",
"guide",
"gw",
"help",
"helpdesk",
"hera",
"heracles",
"hercules",
"home",
"homer",
"hotspot",
"hr",
"hypernova",
"images",
"imail",
"imap",
"imap3",
"imap3d",
"imapd",
"imaps",
"imgs",
"imogen",
"inmuebles",
"internal",
"interno",
"intranet",
"ipsec",
"ipv6",
"irc",
"ircd",
"jabber",
"jupiter",
"lab",
"laboratories",
"laboratorio",
"labs",
"library",
"linux",
"lisa",
"localhost",
"log",
"login",
"logon",
"logs",
"mail",
"mailgate",
"manager",
"marketing",
"member",
"members",
"mercury", // MX server?
"meta",
"meta01",
"meta02",
"meta03",
"meta1",
"meta2",
"meta3",
"miembros",
"minerva",
"mob",
"mobile",
"moodle",
"movil",
"mssql",
"mx",
"mx0",
"mx01",
"mx02",
"mx03",
"mx1",
"mx2",
"mx3",
"mysql",
"nelson",
"neon",
"net",
"netmail",
"news",
"novell",
"ns",
"ns0",
"ns01",
"ns02",
"ns03",
"ns1",
"ns2",
"ns3",
"ntp",
"online",
"ora",
"oracle",
"osx",
"owa",
"partners",
"pcanywhere",
"pegasus",
"pendrell",
"personal",
"photo",
"photos",
"pop",
"pop3",
"portal",
"postgresql",
"postman",
"postmaster",
"pp", // preprod?
"ppp",
"pre-prod",
"preprod",
"private",
"prod",
"proxy",
"prueba",
"pruebas",
"pub",
"public",
"ras",
"remote",
"reports",
"research",
"restricted",
"robinhood",
"router",
"rtr",
"sales",
"sample",
"samples",
"sandbox",
"search",
"secure",
"seguro",
"server",
"services",
"servicios",
"servidor",
"sharepoint",
"shop",
"shopping",
"sms",
"smtp",
"socios",
"solaris",
"soporte",
"sp", // sharepoint?
"sql",
"squirrel",
"squirrelmail",
"ssh",
"staff",
"staging",
"stats",
"sun",
"support",
"test",
"tftp",
"tienda",
"uat",
"unix",
"upload",
"uploads",
"ventas",
"virtual",
"vista",
"vm",
"vnc",
"vpn",
"vpn1",
"vpn2",
"vpn3",
"wap",
"web",
"web0",
"web01",
"web02",
"web03",
"web1",
"web2",
"web3",
"webadmin",
"webct",
"weblog",
"webmail",
"webmaster",
"webmin",
"win",
"windows",
"ww0",
"ww01",
"ww02",
"ww03",
"ww1",
"ww2",
"ww3",
"www",
"www0",
"www01",
"www02",
"www03",
"www1",
"www2",
"www3",
"xanthus",
"zeus"
};
