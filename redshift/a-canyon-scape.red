scene{
    film-settings{color-scale:0.0125; convert-to-srgb:1;}
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
            width:400;
            height:300;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
        }
    }
    cameras{
        aerial{
            transform{
                move-up{1500}
                move-forward{100}
                yaw{0}
                pitch{0}

                move-up{2000}
            }
        }
    }
    objects{
        lazy-quadtree{
                color{0.85;1.0;0.85}
                max-recursion:8;
                lod-factor:0.000125;
                size:50000;
                code:
                        (defun foo (x y)
                                ([LayeredNoise2d
                                        filter{cosine}
                                        seed{5}
                                        frequency{0.0002}
                                        layercount{8}
                                        persistence{0.5}
                                ] (+ 100000 x) (+ 100000 y))
                        )

                        (+ 2000 500 (* 5000  (foo x y)))
                        /*(+ 300 (* (sin (* x 0.001)) (sin (* y 0.001))))*/
                ;
        }
        water-plane{height:2010; }
    }
    backgrounds{
        pss-sunsky {
                turbidity:1.9;
                sun-direction{0.2;0.7;1.0}
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
