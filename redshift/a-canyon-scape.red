scene{
    film-settings{color-scale:0.016; convert-to-srgb:1;}

    render-settings{
        prev-vol{
            width:640;
            height:480;
            samples-per-pixel:1;
            surface-integrator{
                type:redshift;
            }
            volume-integrator{
                type:none;
                step-size:1600;
                cutoff-distance:40000
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
                move-right{10000}
                move-up{250}
                move-forward{-50000}

                pitch{7}
                roll{0}
            }
        }
    }
    objects{
        water-plane{
                color{rgb{0.2;0.4;0.2}}
                height:20;
        }
        lazy-quadtree{
                color{rgb{0.25;0.35;0.25}}
                max-recursion:12;
                lod-factor:0.0125;
                size:1000000;
                code:   /*(* 1000 (cos (* x 0.001)) (cos (* y 0.001)))*/
                        (defun foo (x y)
                                ([LayeredNoise2d
                                        filter{cosine}
                                        seed{3}
                                        frequency{1}
                                        layercount{14}
                                        persistence{0.55}
                                ] x y)
                        )

                        (defun ridged (x y)
                                (- 1 (abs (^ (foo x y) 2)))
                        )

                        (+ 0 (* 2000  (foo (* x 0.0001) (* 0.0001 y))))
                ;
        }
    }
    backgrounds{
        pss-sunsky {
                turbidity:9.0;
                sun-direction{2.0;1.3;1}
        }
    }
    volumes {
        exponential {
                absorption{rgb{0;0;0}}
                out-scatter{rgb{0.01;0.01;0.01}}
                emission{rgb{0.001;0.001;0.001}}
                min{0;1700;0}
                phase-function:0;
                base-factor:1;
                exponent-factor:0.009;
        }
    }
}
