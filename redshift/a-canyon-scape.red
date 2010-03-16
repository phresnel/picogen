scene{
    film-settings{color-scale:0.015; convert-to-srgb:1;}
    render-settings{
        prev-vol{
            width:512;
            height:171;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
            volume-integrator{
                type:single;
                step-size:600;
                cutoff-distance:4000;
            }
        }
        prev-no-vol{
            width:800;
            height:600;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
        }
    }
    cameras{
        aerial{
            transform{
                move-up{320}
                move-forward{8000}
                yaw{0}
                pitch{0}
            }
        }
    }
    objects{
        lazy-quadtree{
                color{1.0;1.0;1.0}
                max-recursion:10;
                lod-factor:0.000125;
                size:500000;
                code:
                        (defun foo (x y)
                                ([LayeredNoise2d
                                        filter{cosine}
                                        seed{5}
                                        frequency{0.0001}
                                        layercount{8}
                                        persistence{0.5}
                                ] (+ 100000 x) (+ 100000 y))
                        )

                        (+ 500 (* 5000  (foo x y)))
                        /*(+ 300 (* (sin (* x 0.001)) (sin (* y 0.001))))*/
                ;
        }
        water-plane{height:0; }
    }
    backgrounds{
        pss-sunsky {
                turbidity:2.2;
                sun-direction{1.0;1.5;1.0}
        }
    }
    volumes {
        exponential {
                absorption{0.0007;0.0007;0.0007}
                out-scatter{0.001;0.001;0.001}
                emission{0.0006;0.0006;0.0006}
                min{0;300;0}
                phase-function:0;
                base-factor:1;
                exponent-factor:0.005;
        }
    }
}
