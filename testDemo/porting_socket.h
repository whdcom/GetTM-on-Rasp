/*************************************************
Copyright (C), 2019-2029, GuideIR Tech. Co., Ltd.
File name	: porting_socket.h
Author		: Guide Sensmart RD
Version		: 1.0
Date		: 2019/7/6
Description	:
*************************************************/

#ifndef	PORTING_SOCKET_H
#define PORTING_SOCKET_H

#include "common_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define GUIDEIR_AF_INET         (2)               // internetwork: UDP, TCP, etc.
#define GUIDEIR_AF_INET6        (23)              // Internetwork Version 6

#define GUIDEIR_SOCK_STREAM     (1)               // stream socket
#define GUIDEIR_SOCK_DGRAM      (2)               // datagram socket

#define SOCKET_FIONBIO			(10)
#define SOCKET_BUFSIZE			(11)
#define SOCKET_TIMEOUT			(12)
#define MULTICAST_LOOP			(13)

#define GD_FD_SETSIZE			(64)
#define INTERRUPT_SYSTEM_CALL	(-2)		//EINTR		 4	/* Interrupted system call */

typedef struct
{
    UINT32_T	fd_count;					/* how many are SET? */
    HANDLE_T	fd_array[GD_FD_SETSIZE];	/* an array of SOCKETs */
} GD_FD_SET;

typedef UINT32_T    GUIDEIR_IPV4;

typedef struct
{
    UINT16_T ip[8];
} GUIDEIR_IPV6;

/* basic IP struct definition, SUPPORT IPV6 */
typedef struct
{
    UINT16_T  version;
    union {
        GUIDEIR_IPV4 ipv4;
        GUIDEIR_IPV6 ipv6;
    } addr;
} GUIDEIR_SOCKADDR;

/**
* @brief	套接字模块初始化，SDK初始化时调用一次，其它模块使用前不用再初始化。支持多次初始化。windows平台需要，linux平台实现为空。
* @param	无
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_init(VOID);

/**
* @brief	套接字模块去初始化，SDK退出前调用。
* @param	无
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_uninit(VOID);

/**
* @brief	创建套接字
* @param
* family	协议族	GUIDEIR_AF_INET, GUIDEIR_AF_INET6.
* type		socket类型	GUIDEIR_SOCK_STREAM, GUIDEIR_SOCK_DGRAM.
* protocol	一般为0。当protocol为0时，会自动选择type类型对应的默认协议。
*			为1时，表示创建组播socket。
* @return	socket句柄，0表示失败。
*/
HANDLE_T porting_socket_open(INT32_T family, INT32_T type, INT32_T protocol);

/**
* @brief	服务端将socket与地址关联
* @param
* sockfd	一个未绑定的socket
* ip		ip地址
* port		端口
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_bind(HANDLE_T sockfd, CONST CHAR_T *ip, UINT16_T port);

/**
* @brief	服务端将socket与地址关联
* @param
* sockfd	一个未绑定的socket
* ipaddr	ip地址，本机字节序
* port		端口，本机字节序
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_bind_ex(HANDLE_T sockfd, GUIDEIR_SOCKADDR ipaddr, UINT16_T port);

/**
* @brief	加入组播
* @param
* sockfd	socket
* family	协议族	GUIDEIR_AF_INET, GUIDEIR_AF_INET6.
* ip    	ip地址，本机字节序
* port		端口，本机字节序
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
HANDLE_T porting_socket_joingroup(HANDLE_T sockfd, INT32_T family, CONST CHAR_T *ip, UINT16_T port);

/**
* @brief	服务端网络监听
* @param
* sockfd	socket描述符，该socket是一个未连接状态的socket
* backlog	挂起连接的最大长度，一般为3-5。指定队列中最多可容纳的等待accept连接数，注意不是最大同时连接个数。
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_listen(HANDLE_T sockfd, INT32_T backlog);

/**
* @brief	服务端connect接收
* @param
* sockfd	listen函数用到的socket
* @return	accept将返回一个新的SOCKET描述符，即新建连接的socket句柄。失败则返回NULL。
*/
HANDLE_T porting_socket_accept(HANDLE_T sockfd);

/**
* @brief	客户端请求服务端连接
* @param
* sockfd	一个未连接状态的socket
* ip		服务器ip地址
* port		服务器端口
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_connect(HANDLE_T sockfd, CONST CHAR_T *ip, UINT16_T port);

/**
* @brief	发送数据
* @param
* sockfd	一个已连接的socket描述符
* buf		数据buffer
* buflen	待发送的数据长度
* flags		一般为0，可以设置以下几个值：
* 1：MSG_DONTROUTE（Specifies that the data should not be subject to routing. A Windows Sockets service provider can choose to ignore this flag.）
* 2：MSG_OOB（Sends OOB data (stream-style socket such as SOCK_STREAM only）
* 3：0
* @return	实际发送的数据长度，这个值可能比参数len小，这也意味着数据缓冲区没有全部发出去，要进行后续处理。出错则返回GUIDEIR_ERR。
*/
INT32_T porting_socket_send(HANDLE_T sockfd, CONST CHAR_T *buf, INT32_T buflen, INT32_T flags);

/**
* @brief	接收数据
* @param
* sockfd	一个已连接的socket描述符
* buf		数据buffer
* buflen	待接收的数据长度
* flags		一般为0，可以设置以下几个值：
* 1：MSG_DONTROUTE（Specifies that the data should not be subject to routing. A Windows Sockets service provider can choose to ignore this flag.）
* 2：MSG_OOB（Sends OOB data (stream-style socket such as SOCK_STREAM only）
* 3：0
* @return	实际接收的数据长度。如果连接已关闭，返回值将是0。出错则返回GUIDEIR_ERR。
*/
INT32_T porting_socket_recv(HANDLE_T sockfd, CHAR_T *buf, INT32_T buflen, INT32_T flags);

/**
* @brief	无连接方式发送数据
* @param
* sockfd	一个无连接的socket描述符
* buf		数据buffer
* buflen	待发送的数据长度
* flags		一般为0
* ip		目标主机ip
* port		目标主机port
* @return	实际发送的数据长度，这个值可能比参数len小，这也意味着数据缓冲区没有全部发出去，要进行后续处理。出错则返回GUIDEIR_ERR。
*/
INT32_T porting_socket_sendto(HANDLE_T sockfd, CONST CHAR_T *buf, INT32_T buflen, INT32_T flags, CONST CHAR_T *ip,	INT32_T port);

/**
* @brief	无连接方式发送数据
* @param
* sockfd	一个无连接的socket描述符
* buf		数据buffer
* buflen	待发送的数据长度
* flags		一般为0
* ipaddr	目标主机ip，本机字节序
* port		目标主机port，本机字节序
* @return	实际发送的数据长度，这个值可能比参数len小，这也意味着数据缓冲区没有全部发出去，要进行后续处理。出错则返回GUIDEIR_ERR。
*/
INT32_T porting_socket_sendto_ex(HANDLE_T sockfd, CONST CHAR_T *buf, INT32_T buflen, INT32_T flags, GUIDEIR_SOCKADDR ipaddr, INT32_T port);

/**
* @brief	无连接方式接收数据
* @param
* sockfd	一个无连接的socket描述符
* buf		数据buffer
* buflen	待接收的数据长度
* flags		一般为0
* ipaddr	发送方ip地址，已转化为本地字节序
* port		发送方端口，已转化为本地字节序
* @return	实际接收的数据长度。如果连接已关闭，返回值将是0。出错则返回GUIDEIR_ERR。
*/
INT32_T porting_socket_recvfrom(HANDLE_T sockfd, CHAR_T *buf, INT32_T buflen, INT32_T flags, GUIDEIR_SOCKADDR *ipaddr, INT32_T *port);

/**
* @brief	获取套接字地址
* @param
* sockfd	socket描述符
* ipaddr	套接字ip地址，已转化为本地字节序
* port		套接字端口，已转化为本地字节序
* @return	0，表示成功。出错则返回GUIDEIR_ERR。
* @note		若一个套接字与INADDR_ANY捆绑，也就是说该套接字可以用任意主机的地址，此时除非调用connect()或accept()来连接，否则getsockname()将不会返回主机IP地址的任何信息。
*			除非套接字被连接，WINDOWS套接字应用程序不应假设IP地址会从INADDR_ANY变成其它地址。
*/
INT32_T porting_socket_getsockname(HANDLE_T sockfd, GUIDEIR_SOCKADDR *ipaddr, INT32_T *port);

/**
* @brief	获取本地主机名
* @param
* name	    输出参数，保存主机名
* len	    输出缓冲区的长度。
* @return	0，表示成功。出错则返回GUIDEIR_ERR。
*/
INT32_T porting_socket_gethostname(CHAR_T *name, INT32_T len);

/**
* @brief	用域名或主机名获取IP地址
* @param
* name		域名或主机名
* ipaddr	输出参数，ip地址，已转化为本地字节序
* size		输出缓冲区能存储ip地址的个数。
* @return	0，表示成功。出错则返回GUIDEIR_ERR。
*/
INT32_T porting_socket_gethostbyname(CONST CHAR_T *name, GUIDEIR_SOCKADDR *ipaddr, INT32_T size);

/**
* @brief	获取套接字的属性
* @param
* sockfd	socket描述符
* level		选项所在的协议层
* optname	需要获取的属性名，缓冲大小SOCKET_BUFSIZE，超时时间SOCKET_TIMEOUT。
* optval	指向保存属性值的缓冲区
* optlen	指向保存属性值的长度
* @return	0，成功。否则，失败。
*/
INT32_T porting_socket_getsockopt(HANDLE_T sockfd, INT32_T level, INT32_T optname, VOID *optval, INT32_T *optlen);

/**
* @brief	设置套接字的属性
* @param
* sockfd	socket描述符
* level		选项所在的协议层
* optname	需要设置的属性名，缓冲大小SOCKET_BUFSIZE，超时时间SOCKET_TIMEOUT。
* optval	指向保存属性值的缓冲区
* optlen	指向保存属性值的长度
* @return	0，成功。否则，失败。
*/
INT32_T porting_socket_setsockopt(HANDLE_T sockfd, INT32_T level, INT32_T optname, VOID *optval, INT32_T optlen);

/**
* @brief	改变套接字属性为非阻塞
* @param
* sockfd	socket描述符
* cmd		命令类型，为 SOCKET_FIONBIO
* arg		命令参数，1表示非阻塞，0表示阻塞。
* @return	0，成功。否则，失败。
*/
INT32_T porting_socket_ioctl(HANDLE_T sockfd, INT32_T cmd, INT32_T arg);

/**
* @brief	确定一个或多个套接字的状态，判断套接字上是否有数据，或者能否向一个套接字写入数据。常用于在一个线程内用非阻塞方式处理多个socket。
* @param
* nfds：    需要检查的文件描述符个数，数值应该比readset、writeset、exceptset中最大数更大，而不是实际的文件描述符总数
* readset： 用来检查可读性的一组文件描述符
* writeset：用来检查可写性的一组文件描述符
* exceptset：用来检查意外状态的文件描述符，错误不属于意外状态
* timeout： 大于0表示等待多少毫秒；等于0表示不等待立即返回；-1表示永久等待。
* @return
* 大于0：	返回处于就绪状态并且已经包含在fd_set结构中的描述字总数
* 等于0：	超时，没有可读写或错误的文件。
* SOCKET_ERROR出错
*/
INT32_T porting_socket_select(INT32_T nfds, GD_FD_SET* readset, GD_FD_SET* writeset, GD_FD_SET* exceptset, INT32_T timeout);

/**
* @brief	禁止掉socket的recv或send行为。为保证数据收发完整性，在关闭socket之前应调用shutdown。
* @param
* sockfd	socket描述符
* what		取值有3个
* 0：不允许在socket上recv数据。
* 1：不允许在socket上send数据。
* 2：不允许在socket上recv和send数据。
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_shutdown(HANDLE_T sockfd, INT32_T what);

/**
* @brief	关闭套接字
* @param
* sockfd	socket描述符
* @return
* GUIDEIR_OK 成功
* GUIDEIR_ERR 失败
*/
INT32_T porting_socket_close(HANDLE_T sockfd);

#ifdef __cplusplus
}
#endif

#endif // !PORTING_SOCKET_H

