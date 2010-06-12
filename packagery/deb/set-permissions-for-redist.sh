$(chown -R root:root DEBS/* 2>/dev/null  \
  && chmod a+x DEBS/*/usr/bin/*          \
) || echo "FAILED. Run me as root."