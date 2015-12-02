systemctl stop nerf_geometry
SERVICE=/etc/systemd/system/nerf_geometry.service
rm -rf $SERVICE
systemctl daemon-reload