TEMPLATE    =   subdirs
CONFIG      += ordered      # Well I know, but no choice...

quickproperties2.file         = src/quickproperties2.pro

sample-propsedit.subdir   = samples/propsedit
sample-propsedit.depends  = quickproperties2

sample-propslist.subdir   = samples/propslist
sample-propslist.depends  = quickproperties2

sample-propslist.subdir   = samples/propslist
sample-propslist.depends  = quickproperties2

sample-container.subdir   = samples/container
sample-container.depends  = quickproperties2

qps2-tests.subdir   = tests
qps2-tests.depends  = quickproperties2

SUBDIRS     +=  quickproperties2 sample-propsedit sample-propslist sample-container qps2-tests 



