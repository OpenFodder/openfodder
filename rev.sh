echo -n 'const char *SVNREV = "' >../rev.h
svn info | grep 'Last Changed Rev:' | awk -F': ' ' {print $2}' | tr -d '\n' >>../rev.h
echo '";' >>../rev.h
echo -n 'const char *SVNDATE = "' >>../rev.h
svn info | grep 'Last Changed Date:' | awk -F': ' ' {print $2}' | tr -d '\n' >>../rev.h
echo '";' >>../rev.h
echo -n >>../rev.h

