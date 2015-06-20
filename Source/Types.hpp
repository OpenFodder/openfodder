
#ifdef WIN64
	typedef unsigned char            uint8;     //!< One byte
	typedef unsigned short           uint16;    //!< Two bytes
	typedef unsigned int             uint32;    //!< Four bytes
	typedef unsigned long long int	 uint64;    //!< Eight bytes

	typedef signed char              int8;      //!< One byte
	typedef signed short             int16;     //!< Two bytes
	typedef signed int               int32;     //!< Four bytes
	typedef signed long long int     int64;     //!< Eight bytes


#else
	typedef unsigned char            uint8;     //!< One byte
	typedef unsigned short           uint16;    //!< Two bytes
	typedef unsigned int             uint32;    //!< Four bytes
	typedef unsigned long			 uint64;    //!< Eight bytes

	typedef signed char              int8;      //!< One byte
	typedef signed short             int16;     //!< Two bytes
	typedef signed int               int32;     //!< Four bytes
	typedef signed long			     int64;     //!< Eight bytes

#endif
