
class LineStyle

  attr_reader :id, :opts

  def initialize( id, opts = {} )
    @id = id
    @opts = opts
  end

  def to_plot( plot )
    str =  "style line #{id}"
    str += " lt #{opts[:lt]}" if opts[:lt]
    str += " lc #{opts[:lc]}" if opts[:lc]
    str += " lw #{opts[:lw]}" if opts[:lw]
    str += " pt #{opts[:pt]}" if opts[:pt]
    str += " ps #{opts[:ps]}" if opts[:ps]
    str += " lc rgb \"#{opts[:rgb]}\"" if opts[:rgb]

    plot.set str
  end
end

class LineStyles
  def initialize
    @styles = Hash.new {|h,k| h[k] = LineStyle.new( id_and_increment ) }
    @id = 10
  end

  def id_and_increment
    foo = @id
    @id += 1
    foo
  end

  def style( name )
    key = @styles.keys.find { |key| key.match name }
    key ? @styles[key]  : @styles[name]
  end
  alias_method :[], :style

  def []=(a,opts)
    @styles[a] = LineStyle.new( id_and_increment, opts )
  end

  def to_plot( plot )
    @styles.each_value { |style|
      style.to_plot( plot )
    }
  end
end


def per_algorithm_by_axis_bias( pair, presults, axis )

  #  biases = presults.map { |result|
  #    result.homography.opts[:roll_bias] if result.homography.opts
  #  }.compact.uniq
  #
  #  bmin = 0
  #  bmax = biases.max

  presults = presults.select { |result|
    result.homography.opts and
      result.homography.name.match axis.to_s
  }

  Gnuplot::do_plot( plot_path( pair, "per_algorithm_by_%s.tex" % axis ) ) { |plot|

    styles = LineStyles.new

    style = { rgb: "#ff0000" }
    styles["BruteForceMatcher"] = style.merge( lt: 1 )
    styles[/^BruteForceRatioMatcher/] = style.merge( lt: 5 )

    style = { lt: 1, lw: 2 }
    styles[/^ExtendedBrute.*00\)/] = style.merge( rgb: "#00ff00" )
    styles[/^ExtendedBrute.*02\)/] = style.merge( rgb: "#00bb00" )
    styles[/^ExtendedBrute.*04\)/] = style.merge( rgb: "#008800" )
    styles[/^ExtendedBrute.*06\)/] = style.merge( rgb: "#003300" )

    style = { lt: 1, lw: 2 }
    styles[/^ExtendedKdTreeFlannMatcher.*00\)/] = style.merge( rgb: "#0000ff" ) 
    styles[/^ExtendedKdTreeFlannMatcher.*02\)/] = style.merge( rgb: "#0000bb" )
    styles[/^ExtendedKdTreeFlannMatcher.*04\)/] = style.merge( rgb: "#000088" )
    styles[/^ExtendedKdTreeFlannMatcher.*06\)/] = style.merge( rgb: "#000033" )

    style = { lt: 5, lw: 2 }
    styles[/^ExtendedKdTreeFlannRatioMatcher.*00,r/] = style.merge( rgb: "#0000ff" ) 
    styles[/^ExtendedKdTreeFlannRatioMatcher.*02,r/] = style.merge( rgb: "#0000bb" )
    styles[/^ExtendedKdTreeFlannRatioMatcher.*04,r/] = style.merge( rgb: "#000088" )
    styles[/^ExtendedKdTreeFlannRatioMatcher.*06,r/] = style.merge( rgb: "#000033" )

    style = { rgb: "#ff00ff", lw: 2 }
    styles[/^ManualGeometry /] = style.merge( lt: 1 )
    styles[/^ManualGeometryRatio/] = style.merge( lt: 5 )

    partition = presults.partition_by_algorithm_description

    partition.keys.sort { |a,b|
      a <=> b
    }.each { |algo|

      paresults = partition[algo]
      # Filter out some of the non-geometry-based matchers
      next if algo == "KMeansFlannMatcher" or
        algo =~ /^KMeansFlannRatioMatcher/ or
        algo == "KdTreeFlannMatcher" or
        algo =~ /^KdTreeFlannRatioMatcher/



      data = paresults.map { |res|
        if res.homography.name == "ref"
          [ 0, res.frac_inliers ]
        else
          [ res.homography.send(axis), res.frac_inliers ]
        end
      }.sort { |a,b| b.first<=> a.first }

      #      data = case paresults.length
      #             when 1
      #               result = pharesults.first
      #               [ [ wmin, result.frac_inliers ],
      #                 [ wmax, result.frac_inliers ] ]
      #             else
      #               pharesults.map { |res|[ res.algo.opts[:weight], res.frac_inliers ] }
      #             end
      #
      plot.data << Plot::DataSet.new( data.transpose ) do |ds|
        ds.with = "lines"
        ds.using = "1:2 linestyle #{styles[algo].id}"
        ds.title = algo
      end
    }

    safe_pair = pair.to_s.gsub(/_/, ' ')
    safe_axis = axis.to_s.gsub(/_/, ' ')

    styles.to_plot( plot )

    # Set some default styles
    plot.set "style line 80 lt 1 lc rgb '#000000'"
    plot.set "style line 81 lt 3 lc rgb '#808080' lw 0.5"
    plot.set "grid back linestyle 81"
    plot.set "border 3 back linestyle 80"
    plot.set "xtics nomirror"
    plot.set "ytics nomirror"


    plot.terminal "epslatex size 10,5 color font \"default\" 7" 
    #plot.terminal "svg size 1600,1024"
    plot.title "Fraction of inliers vs. %s for pair %s" % [safe_axis, safe_pair]
    plot.xlabel "%s (rad)" % safe_axis
    plot.ylabel "Frac. inliers"
    plot.yrange "[0:1.05]"
    plot.set "key outside"
  }
end


