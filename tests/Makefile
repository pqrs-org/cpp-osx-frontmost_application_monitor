all:
	@for d in `find * -type d`; do \
		if [ -f "$$d/project.yml" ]; then \
			echo "[Test] $$d"; \
			make -C $$d || exit 1; \
		fi; \
	done

clean:
	@for d in `find * -type d`; do \
		if [ -f "$$d/project.yml" ]; then \
			make -C $$d clean; \
		fi; \
	done
